#include "UdpGameServer.hpp"
#include <iostream>
#include <cstring>

namespace {
    uint16_t readUint16BE(const uint8_t* buf) {
        return (static_cast<uint16_t>(buf[0]) << 8) | buf[1];
    }

    void writeUint16BE(uint8_t* buf, uint16_t value) {
        buf[0] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buf[1] = static_cast<uint8_t>(value & 0xFF);
    }
}

UdpGameServer::UdpGameServer(unsigned short port)
    : socket_(ioContext_, udp::endpoint(udp::v4(), port))
    , running_(false)
{
}

UdpGameServer::~UdpGameServer()
{
    stop();
}

void UdpGameServer::start()
{
    if (running_) return;
    running_ = true;

    doReceive();

    ioThread_ = std::thread([this]() {
        ioContext_.run();
    });

    std::cout << "[UdpGameServer] Started on port " 
              << socket_.local_endpoint().port() << std::endl;
}

void UdpGameServer::stop()
{
    if (!running_) return;
    running_ = false;

    ioContext_.stop();
    if (ioThread_.joinable()) {
        ioThread_.join();
    }
    socket_.close();

    std::cout << "[UdpGameServer] Stopped.\n";
}

void UdpGameServer::doReceive()
{
    if (!running_) return;

    socket_.async_receive_from(
        boost::asio::buffer(recvBuffer_),
        remoteEndpoint_,
        [this](boost::system::error_code ec, std::size_t bytesTransferred) {
            if (!ec && bytesTransferred > 0) {
                std::vector<uint8_t> pkt(recvBuffer_.begin(), recvBuffer_.begin() + bytesTransferred);
                handlePacket(pkt, remoteEndpoint_);
            }
            if (running_) {
                doReceive();
            }
        }
    );
}

void UdpGameServer::handlePacket(const std::vector<uint8_t>& data, const udp::endpoint& from)
{
    if (data.size() < 3) {
        return;
    }

    uint8_t msgType = data[0];
    uint16_t seq = readUint16BE(&data[1]);

    switch (msgType) {
    case 0xA0:
        handleAck(seq, from);
        break;

    case 0x01:
        handleAddObject({data.begin() + 3, data.end()}, seq, from);
        break;
    case 0x02:
        handleRemoveObject({data.begin() + 3, data.end()}, seq, from);
        break;
    case 0x03:
        handleUpdateObject({data.begin() + 3, data.end()}, seq, from);
        break;
    case 0x10:
        handleDbInsert({data.begin() + 3, data.end()}, seq, from);
        break;
    case 0x11:
        handleDbRemove({data.begin() + 3, data.end()}, seq, from);
        break;
    case 0x12:
        handleDbUpdate({data.begin() + 3, data.end()}, seq, from);
        break;

    default:
        std::cout << "[UdpGameServer] Unknown msgType=" << (int)msgType << ", seq=" << seq << std::endl;
        break;
    }
}

void UdpGameServer::handleAck(uint16_t seq, const udp::endpoint& from)
{
    ackedSequences_[seq] = true; 
    std::cout << "[UdpGameServer] Received ACK for seq=" << seq 
              << " from " << from.address().to_string() << std::endl;
}

void UdpGameServer::sendAck(uint16_t seq, const udp::endpoint& to)
{
    std::vector<uint8_t> ackPacket(3);
    ackPacket[0] = 0xA0;
    writeUint16BE(&ackPacket[1], seq);
    sendPacket(ackPacket, to);
}

void UdpGameServer::sendPacket(const std::vector<uint8_t>& data, const udp::endpoint& to)
{
    socket_.async_send_to(
        boost::asio::buffer(data),
        to,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytesSent*/){

        }
    );
}

// payload: [ <objectNameLen=1>, objectNameData... ]
void UdpGameServer::handleAddObject(const std::vector<uint8_t>& payload, uint16_t seq, const udp::endpoint& from)
{
    if (payload.empty()) {
        return;
    }

    uint8_t nameLen = payload[0];

    if (payload.size() < 1 + nameLen) {
        return;
    }

    std::string objName((const char*)&payload[1], (const char*)&payload[1] + nameLen);

    engine::Object newObj;
    newObj.setName(objName);

    auto &ref = gameManager_.addObject(newObj);
    std::cout << "[UdpGameServer] Added object name=" << ref.getName() 
              << " (seq=" << seq << ")\n";

    sendAck(seq, from);
}

// payload: [ <index:4 bytes> ]
void UdpGameServer::handleRemoveObject(const std::vector<uint8_t>& payload, uint16_t seq, const udp::endpoint& from)
{
    if (payload.size() < 4) return;

    uint32_t index = (payload[0] << 24) | (payload[1] << 16)
                   | (payload[2] << 8 ) |  payload[3];

    bool removed = gameManager_.removeObject(index);
    if (removed) {
        std::cout << "[UdpGameServer] Removed object index=" << index << "\n";
    } else {
        std::cout << "[UdpGameServer] Failed removing index=" << index << "\n";
    }

    sendAck(seq, from);
}

// payload: [ <index:4 bytes>, <newNameLen=1>, newNameData... ]
void UdpGameServer::handleUpdateObject(const std::vector<uint8_t>& payload, uint16_t seq, const udp::endpoint& from)
{
    if (payload.size() < 5) return;
    uint32_t index = (payload[0] << 24) | (payload[1] << 16)
                   | (payload[2] << 8 ) |  payload[3];

    uint8_t nameLen = payload[4];
    if (payload.size() < 5 + nameLen) return;

    std::string newName((const char*)&payload[5], (const char*)&payload[5] + nameLen);

    engine::Object *objPtr = gameManager_.getObject(index);
    if (!objPtr) {
        sendAck(seq, from);
        return;
    }
    engine::Object newObj(*objPtr);
    newObj.setName(newName);

    gameManager_.updateObject(index, newObj);
    std::cout << "[UdpGameServer] Updated object index=" << index
              << " newName=" << newName << "\n";

    sendAck(seq, from);
}

// payload: [ <key:4 bytes>, <valueLen=1>, valueData... ] (assuming string)
void UdpGameServer::handleDbInsert(const std::vector<uint8_t>& payload, uint16_t seq, const udp::endpoint& from)
{
    if (payload.size() < 5) return;
    uint32_t key = (payload[0] << 24) | (payload[1] << 16)
                 | (payload[2] << 8 ) |  payload[3];
    uint8_t valLen = payload[4];
    if (payload.size() < 5 + valLen) return;
    std::string strVal((const char*)&payload[5], (const char*)&payload[5] + valLen);

    gameManager_.dbInsert(key, strVal);
    std::cout << "[UdpGameServer] DBInsert key=" << key << ", value=\"" << strVal << "\"\n";

    sendAck(seq, from);
}

// payload: [ <key:4 bytes> ]
void UdpGameServer::handleDbRemove(const std::vector<uint8_t>& payload, uint16_t seq, const udp::endpoint& from)
{
    if (payload.size() < 4) return;
    uint32_t key = (payload[0] << 24) | (payload[1] << 16)
                 | (payload[2] << 8 ) |  payload[3];
    gameManager_.dbRemove(key);

    std::cout << "[UdpGameServer] DBRemove key=" << key << "\n";
    sendAck(seq, from);
}

// payload: [ <key:4 bytes>, <valLen=1>, valData... ]
void UdpGameServer::handleDbUpdate(const std::vector<uint8_t>& payload, uint16_t seq, const udp::endpoint& from)
{
    if (payload.size() < 5) return;
    uint32_t key = (payload[0] << 24) | (payload[1] << 16)
                 | (payload[2] << 8 ) |  payload[3];
    uint8_t valLen = payload[4];
    if (payload.size() < 5 + valLen) return;
    std::string strVal((const char*)&payload[5], (const char*)&payload[5] + valLen);

    gameManager_.dbUpdate(key, strVal);
    std::cout << "[UdpGameServer] DBUpdate key=" << key 
              << ", newVal=\"" << strVal << "\"\n";
    sendAck(seq, from);
}
