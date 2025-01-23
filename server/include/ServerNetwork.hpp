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

private:
    boost::asio::io_context &_ioContext;

    boost::asio::ip::tcp::acceptor _acceptor;
    std::string _ipAddress;
    unsigned short _tcpPort;

    boost::asio::ip::udp::socket _udpSocket;
    unsigned short _udpPort;

    std::unordered_map<std::uint64_t, std::shared_ptr<ClientSession>> _sessions;
    std::atomic<std::uint64_t> _nextSessionId{1};

    bool _running;
};

#endif
