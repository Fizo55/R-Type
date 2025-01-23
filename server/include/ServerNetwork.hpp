#ifndef NETWORK_SERVER_HPP_
#define NETWORK_SERVER_HPP_

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <string>

static const std::uint16_t PROTOCOL_MAGIC = 0xEB27;

enum class NetworkCmd : std::uint8_t {
    HELLO   = 0x01,
    GOODBYE = 0x02
};

struct NetworkMessage {
    std::uint16_t magic;
    NetworkCmd command;
    std::uint16_t length;
    std::vector<char> payload;
};

struct UdpSessionState {
    std::uint32_t lastSequenceReceived;
};

enum class UdpCmd : std::uint8_t {
    CLIENT_UPDATE = 0x01,
    ACK           = 0x02
};

struct UdpHeader {
    std::uint16_t magic;
    std::uint8_t  cmd;
    std::uint32_t sequenceNumber;
    std::uint32_t ackNumber;
    std::uint16_t length;
};

struct EndpointHash {
    std::size_t operator()(const boost::asio::ip::udp::endpoint &ep) const {
        auto addr = ep.address().to_string();
        std::size_t h1 = std::hash<std::string>()(addr);
        std::size_t h2 = std::hash<unsigned short>()(ep.port());
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};


struct ClientSession {
    std::uint64_t id;
    std::string name;
    boost::asio::ip::tcp::socket tcpSocket;

    ClientSession(boost::asio::io_context& io)
      : id(0), name(""), tcpSocket(io)
    {
    }
};

class NetworkServer : public std::enable_shared_from_this<NetworkServer> {
public:
    NetworkServer(boost::asio::io_context &io,
                  const std::string &ipAddress,
                  unsigned short tcpPort,
                  unsigned short udpPort);

    void start();
    void stop();

private:
    void doAccept();
    void onAccept(std::shared_ptr<ClientSession> session,
                  const boost::system::error_code &ec);

    void startReadTcp(std::shared_ptr<ClientSession> session);
    void onReadHeader(std::shared_ptr<ClientSession> session,
                      std::shared_ptr<std::vector<char>> buffer,
                      const boost::system::error_code &ec,
                      std::size_t bytesTransferred);
    void onReadPayload(std::shared_ptr<ClientSession> session,
                       NetworkMessage &msg,
                       const boost::system::error_code &ec,
                       std::size_t bytesTransferred);

    void handleHello(std::shared_ptr<ClientSession> session, const std::vector<char> &payload);
    void handleGoodbye(std::shared_ptr<ClientSession> session);

    bool parseHeader(const char *data, std::size_t size, NetworkMessage &outMsg);

    void removeSession(std::uint64_t sessionId);

    std::unordered_map<boost::asio::ip::udp::endpoint, UdpSessionState, EndpointHash> _udpSessions;

    void startReceiveUdp();
    void onReceiveUdp(const boost::system::error_code &ec, std::size_t bytesTransferred);

    bool parseUdpHeader(const char *data, std::size_t size, UdpHeader &outHdr);

    void handleUdpPacket(const UdpHeader &hdr,
        const std::vector<char> &payload,
        const boost::asio::ip::udp::endpoint &sender);

    void sendUdpAck(std::uint32_t seqNumber, std::uint32_t ackNumber,
                    const boost::asio::ip::udp::endpoint &dest);

private:
    boost::asio::io_context &_ioContext;

    boost::asio::ip::tcp::acceptor _acceptor;
    std::string _ipAddress;
    unsigned short _tcpPort;

    boost::asio::ip::udp::socket _udpSocket;
    unsigned short _udpPort;

    std::array<char, 2048> _udpBuffer;
    boost::asio::ip::udp::endpoint _udpRemoteSender;

    std::unordered_map<std::uint64_t, std::shared_ptr<ClientSession>> _sessions;
    std::atomic<std::uint64_t> _nextSessionId{1};

    bool _running;
};

#endif
