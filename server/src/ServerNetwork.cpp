#include "ServerNetwork.hpp"
#include <iostream>
#include <cstring>

NetworkServer::NetworkServer(boost::asio::io_context &io,
                             const std::string &ipAddress,
                             unsigned short tcpPort,
                             unsigned short udpPort,
                             engine::Game &game,
                             engine::ScriptOrchestrator &orchestrator,
                             std::function<engine::ObjectRef()> createPlayerFunc)
    : _ioContext(io)
    , _acceptor(io)
    , _ipAddress(ipAddress)
    , _tcpPort(tcpPort)
    , _udpSocket(io)
    , _udpPort(udpPort)
    , _running(false)
    , _game(game)
    , _orchestrator(orchestrator)
    , _createPlayerFunc(std::move(createPlayerFunc))
{
    _udpBuffer.fill(0);
}

void NetworkServer::start()
{
    _running = true;

    boost::system::error_code ec;
    auto address = boost::asio::ip::make_address(_ipAddress, ec);
    if (ec) {
        std::cerr << "[Server] Invalid IP address: " << _ipAddress 
                  << " => " << ec.message() << "\n";
        return;
    }

    boost::asio::ip::tcp::endpoint tcpEp(address, _tcpPort);
    _acceptor.open(tcpEp.protocol(), ec);
    if (ec) {
        std::cerr << "[Server] TCP open failed: " << ec.message() << "\n";
        return;
    }
    _acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    _acceptor.bind(tcpEp, ec);
    if (ec) {
        std::cerr << "[Server] TCP bind error: " << ec.message() << "\n";
        return;
    }
    _acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "[Server] TCP listen error: " << ec.message() << "\n";
        return;
    }

    doAccept();

    boost::asio::ip::udp::endpoint udpEp(address, _udpPort);
    _udpSocket.open(udpEp.protocol(), ec);
    if (ec) {
        std::cerr << "[Server] UDP open failed: " << ec.message() << "\n";
        return;
    }
    _udpSocket.set_option(boost::asio::socket_base::reuse_address(true), ec);
    _udpSocket.bind(udpEp, ec);
    if (ec) {
        std::cerr << "[Server] UDP bind error: " << ec.message() << "\n";
        return;
    }

    startReceiveUdp();

    std::cout << "[Server] Started on " << _ipAddress 
              << ", TCP=" << _tcpPort << " UDP=" << _udpPort << "\n";
}

void NetworkServer::stop()
{
    if (!_running) return;
    _running = false;

    boost::system::error_code ec;
    _acceptor.close(ec);
    _udpSocket.close(ec);

    for (auto &kv : _sessions) {
        auto &session = kv.second;
        session->tcpSocket.close(ec);
    }
    _sessions.clear();
    std::cout << "[Server] Stopped\n";
}

void NetworkServer::doAccept()
{
    auto session = std::make_shared<ClientSession>(_ioContext);
    session->id = _nextSessionId.fetch_add(1, std::memory_order_relaxed);

    _acceptor.async_accept(
        session->tcpSocket,
        [this, session](const boost::system::error_code &ec)
        {
            onAccept(session, ec);
        }
    );
}

void NetworkServer::onAccept(std::shared_ptr<ClientSession> session,
                             const boost::system::error_code &ec)
{
    if (!ec) {
        std::cout << "[Server] New connection, ID=" << session->id << "\n";
        _sessions[session->id] = session;

        startReadTcp(session);

        if (_running) {
            doAccept();
        }
    } else {
        std::cerr << "[Server] Accept error: " << ec.message() << "\n";
    }
}

void NetworkServer::startReadTcp(std::shared_ptr<ClientSession> session)
{
    auto headerBuf = std::make_shared<std::vector<char>>(5);

    boost::asio::async_read(
        session->tcpSocket,
        boost::asio::buffer(*headerBuf),
        [this, session, headerBuf](const boost::system::error_code &ec,
                                   std::size_t bytesTransferred)
        {
            onReadHeader(session, headerBuf, ec, bytesTransferred);
        }
    );
}

void NetworkServer::onReadHeader(std::shared_ptr<ClientSession> session,
                                 std::shared_ptr<std::vector<char>> buffer,
                                 const boost::system::error_code &ec,
                                 std::size_t bytesTransferred)
{
    if (ec || bytesTransferred < 5) {
        removeSession(session->id);
        return;
    }

    NetworkMessage msg;
    if (!parseHeader(buffer->data(), buffer->size(), msg)) {
        std::cerr << "[Server] Invalid header from client " << session->id << "\n";
        removeSession(session->id);
        return;
    }

    if (msg.length == 0) {
        onReadPayload(session, msg, {}, 0);
    } else {
        auto payloadBuf = std::make_shared<std::vector<char>>(msg.length);
        boost::asio::async_read(
            session->tcpSocket,
            boost::asio::buffer(*payloadBuf),
            [this, session, msg, payloadBuf](const boost::system::error_code &ec2,
                                             std::size_t bytesTransferred2) mutable
            {
                if (!ec2 && bytesTransferred2 == msg.length) {
                    std::copy(payloadBuf->begin(), payloadBuf->end(), std::back_inserter(msg.payload));
                }
                onReadPayload(session, msg, ec2, bytesTransferred2);
            }
        );
    }
}

void NetworkServer::onReadPayload(std::shared_ptr<ClientSession> session,
                                  NetworkMessage &msg,
                                  const boost::system::error_code &ec,
                                  std::size_t bytesTransferred)
{
    if (ec) {
        removeSession(session->id);
        return;
    }

    switch (msg.command) {
    case NetworkCmd::HELLO: {
        handleHello(session, msg.payload);
        break;
    }
    case NetworkCmd::GOODBYE: {
        handleGoodbye(session);
        break;
    }
    default:
        std::cerr << "[Server] Unknown command 0x"
                  << std::hex << (int)msg.command << std::dec
                  << " from client " << session->id << "\n";
        break;
    }

    if (session->tcpSocket.is_open()) {
        startReadTcp(session);
    }
}

void NetworkServer::handleHello(std::shared_ptr<ClientSession> session,
                                const std::vector<char>& payload)
{
    std::string name(payload.begin(), payload.end());
    session->name = name;
    std::cout << "[Server] Client " << session->id
              << " logged in with name: " << name << "\n";

    auto player = _game.buildObjectRef(_createPlayerFunc(), "player" + std::to_string(session->id));
    _game.loadObject(player);
    _orchestrator.fromObject(_game.getFactory(), player);

    std::string idStr = std::to_string(session->id);

    boost::system::error_code ec;
    boost::asio::write(session->tcpSocket, boost::asio::buffer(idStr), ec);

    if (ec) {
        std::cerr << "[Server] Error sending session ID: " << ec.message() << "\n";
    } else {
        std::cout << "[Server] Sent session ID " << idStr
                  << " to client " << session->id << "\n";
    }
}

void NetworkServer::handleGoodbye(std::shared_ptr<ClientSession> session)
{
    std::cout << "[Server] Client " << session->id << " logged out\n";
    removeSession(session->id);
}

bool NetworkServer::parseHeader(const char *data, std::size_t size, NetworkMessage &outMsg)
{
    if (size < 5) return false;
    std::memcpy(&outMsg.magic, data, 2);
    outMsg.command = static_cast<NetworkCmd>(data[2]);
    std::memcpy(&outMsg.length, data + 3, 2);

    if (outMsg.magic != PROTOCOL_MAGIC) {
        return false;
    }
    return true;
}

void NetworkServer::removeSession(std::uint64_t sessionId)
{
    auto &gameObjects = this->_game.getLoadedObjects();
    auto tempPos = std::find_if(gameObjects.begin(), gameObjects.end(), [&](const engine::Object* item) {
        return (item->getName().compare("player" + std::to_string(sessionId)) == 0);
    });
    
    if (tempPos == gameObjects.end()) {
        return;
    }

    auto obj = *tempPos;
    this->_orchestrator.removeScript((std::size_t)obj);
    this->_game.unloadObject(obj);

    auto it = _sessions.find(sessionId);
    if (it != _sessions.end()) {
        boost::system::error_code ec;
        it->second->tcpSocket.close(ec);
        _sessions.erase(it);
        std::cout << "[Server] Removed client session " << sessionId << "\n";
    }
}

void NetworkServer::startReceiveUdp()
{
    _udpSocket.async_receive_from(
        boost::asio::buffer(_udpBuffer),
        _udpRemoteSender,
        [this](const boost::system::error_code &ec, std::size_t bytesTransferred)
        {
            onReceiveUdp(ec, bytesTransferred);
        }
    );
}

void NetworkServer::onReceiveUdp(const boost::system::error_code &ec,
                                 std::size_t bytesTransferred)
{
    if (!ec && bytesTransferred >= sizeof(UdpHeader)) {
        UdpHeader hdr;
        if (parseUdpHeader(_udpBuffer.data(), bytesTransferred, hdr)) {
            std::vector<char> payload;
            if (hdr.length > 0 && (sizeof(UdpHeader) + hdr.length) <= bytesTransferred) {
                payload.insert(payload.end(),
                               _udpBuffer.begin() + sizeof(UdpHeader),
                               _udpBuffer.begin() + sizeof(UdpHeader) + hdr.length);
            }
            handleUdpPacket(hdr, payload, _udpRemoteSender);
        }
    }
    if (_running) {
        startReceiveUdp();
    }
}

bool NetworkServer::parseUdpHeader(const char *data, std::size_t size, UdpHeader &outHdr)
{
    if (size < sizeof(UdpHeader)) return false;

    std::memcpy(&outHdr.magic, data, 2);
    std::memcpy(&outHdr.cmd, data+2, 1);
    std::memcpy(&outHdr.sequenceNumber, data+3, 4);
    std::memcpy(&outHdr.ackNumber, data+7, 4);
    std::memcpy(&outHdr.length, data+11,2);

    if (outHdr.magic != 0xDEAD) {
        return false;
    }
    return true;
}

void NetworkServer::handleUdpPacket(const UdpHeader &hdr,
                                    const std::vector<char> &payload,
                                    const boost::asio::ip::udp::endpoint &sender)
{
    auto &session = _udpSessions[sender];

    if (hdr.sequenceNumber <= session.lastSequenceReceived) {
        std::cout << "[Server][UDP] Duplicate or old packet seq=" << hdr.sequenceNumber
                  << " from " << sender << std::endl;
        return;
    }

    session.lastSequenceReceived = hdr.sequenceNumber;

    switch ((UdpCmd)hdr.cmd) {
        case UdpCmd::CLIENT_UPDATE: {
            std::cout << "[Server][UDP] new packet seq=" << hdr.sequenceNumber
                      << " ack=" << hdr.ackNumber
                      << " from " << sender << ", payload size=" << payload.size() << "\n";
            break;
        // TODO : message
        }
        default: {
            std::cout << "[Server][UDP] Unknown udp cmd=0x"
                      << std::hex << (int)hdr.cmd << std::dec
                      << " from " << sender << "\n";
            break;
        }
    }

    sendUdpAck(hdr.sequenceNumber, 0, sender);
}

void NetworkServer::sendUdpAck(std::uint32_t seqNumber, std::uint32_t ackNumber,
                               const boost::asio::ip::udp::endpoint &dest)
{
    UdpHeader hdr;
    hdr.magic = 0xDEAD;
    hdr.cmd = (std::uint8_t)UdpCmd::ACK;
    hdr.sequenceNumber = seqNumber;
    hdr.ackNumber = ackNumber;
    hdr.length = 0;

    std::vector<char> buffer;
    buffer.resize(sizeof(UdpHeader));
    std::memcpy(buffer.data(), &hdr.magic, 2);
    std::memcpy(buffer.data()+2, &hdr.cmd, 1);
    std::memcpy(buffer.data()+3, &hdr.sequenceNumber, 4);
    std::memcpy(buffer.data()+7, &hdr.ackNumber, 4);
    std::memcpy(buffer.data()+11,&hdr.length, 2);

    _udpSocket.async_send_to(
        boost::asio::buffer(buffer),
        dest,
        [dest](const boost::system::error_code &ec, std::size_t bytesSent)
        {
            if (ec) {
                std::cerr << "[Server][UDP] sendAck error to " << dest
                          << ": " << ec.message() << std::endl;
            }
        }
    );
}
