#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>
#include <yaml-cpp/yaml.h>

using namespace engine;

client::client(const std::string &configPath)
  : _running(false)
  , _tcpSocket(std::make_unique<boost::asio::ip::tcp::socket>(_ioContext))
  , _loginCompleted(false)
  , _delimiter("\r\n\r\n")
{
    engine::ScriptTypeDefinitor<Game> *gameDefinitor = new engine::ScriptTypeDefinitor<Game>();
    engine::ScriptTypeDefinitor<grw::clock> *clockDefinitor = new engine::ScriptTypeDefinitor<grw::clock>();
    engine::ScriptTypeDefinitor<engine::displayManager> *displayManagerDefinitor = new engine::ScriptTypeDefinitor<engine::displayManager>();

    this->_factory.registerComponent<engine_components::Position>();
    this->_factory.registerComponent<engine_components::Size>();
    this->_factory.registerComponent<engine_components::Sprite>();
    this->_factory.registerComponent<engine_components::Script>();
    this->_factory.registerComponent<engine_components::Volatile>();

    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("position", engine_components::Position::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("size", engine_components::Size::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("sprite", engine_components::Sprite::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("script", engine_components::Script::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("volatile", engine_components::Volatile::buildComponent));

#ifdef _WIN32
    std::string basePath = std::filesystem::current_path().string() + "\\";
#else
    std::string basePath = "";
#endif

    YAML::Node gameData = YAML::LoadFile(configPath);

    if (gameData["images"]) {
        for (YAML::const_iterator it = gameData["images"].begin(); it != gameData["images"].end(); ++it) {
            this->_displayManager.registerAsset(it->first.as<std::string>(), basePath + it->second.as<std::string>());
        }
    }

    if (gameData["scripts"]) {
        for (YAML::const_iterator it = gameData["scripts"].begin(); it != gameData["scripts"].end(); ++it) {
            this->_orchestrator.registerScript(it->first.as<std::string>(), basePath + it->second.as<std::string>());
        }
    }

    if (gameData["objects"]) {
        for (const auto &item : gameData["objects"]) {
            this->_game.registerObject(basePath + item.as<std::string>());
        }
    }

    if (gameData["scenes"]) {
        for (const auto &item : gameData["scenes"]) {
            this->_game.registerScene(basePath + item.as<std::string>());
        }
    }

    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)gameDefinitor, "game", "game", (void *)&(this->_game)));
    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)clockDefinitor, "clock", "clock", (void *)&(this->_clock)));
    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)displayManagerDefinitor, "display_manager", "renderer", (void *)&(this->_displayManager)));
    this->_orchestrator.addBinding(clock_register);
    this->_orchestrator.addBinding(display_manager_register);

    this->_game.addFactory(&this->_factory);

    if (gameData["entryScene"])
        this->_game.loadSceneClient(gameData["entryScene"].as<std::string>());

    this->_gameWindow = this->_displayManager.createWindow(800, 600);

    this->_game.writeDBInt(0xF1, (std::int64_t)this->_gameWindow);
    // this->_game.writeDBInt(0x00, (int64_t)this->_game.buildObjectRef(createPlayer(), "player0"));

    // this->_game.loadObject((Object *)this->_game.readDBInt(0x00));

    this->_orchestrator.fromGameObject(this->_game);
}

client::~client()
{

}

void client::connect(const std::string &serverIp, unsigned short serverPort)
{
    boost::system::error_code ec;
    boost::asio::ip::tcp::resolver resolver(_ioContext);
    auto endpoints = resolver.resolve(serverIp, std::to_string(serverPort), ec);

    if (ec) {
        std::cerr << "[Client] Failed to resolve " << serverIp << ":" << serverPort
            << " => " << ec.message() << std::endl;
    }

    else {
        boost::asio::connect(*_tcpSocket, endpoints, ec);
        if (ec) {
            std::cerr << "[Client] Connect error: " << ec.message() << std::endl;
        }
        else {
            std::cout << "[Client] Connected to " << serverIp << ":" << serverPort << std::endl;
        }
    }
}

void client::event(void)
{
    this->_running = !this->_displayManager.event();

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_Z) {

    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_S) {

    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_Q) {

    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_D) {

    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_SPACE) {

    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_Z) {

    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_Q) {

    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_S) {

    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_D) {

    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_SPACE) {

    }
}

void client::start_read_size() {
    _readState = ReadState::READING_SIZE;
    boost::asio::async_read(*_tcpSocket,
        boost::asio::buffer(_sizeBuffer),
        boost::asio::transfer_exactly(_sizeBuffer.size()),
        [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            handle_read_size(ec, bytes_transferred);
        });
}

void client::handle_read_size(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    std::cout << "handle_read_size called\n";
    if (ec) {
        std::cerr << "[Client] Error reading object size: " << ec.message() << "\n";
        return;
    }

    if (bytes_transferred != _sizeBuffer.size()) {
        std::cerr << "[Client] Incomplete size received: " << bytes_transferred << " bytes\n";
        return;
    }

    std::uint32_t network_size;
    std::memcpy(&network_size, _sizeBuffer.data(), sizeof(network_size));
    _objectSize = ntohl(network_size);

    std::cout << "[Client] Received object size: " << _objectSize << " bytes\n";

    if (_objectSize == 0) {
        std::cerr << "[Client] Received object size of 0!\n";
        return;
    }

    _objectBuffer.clear();
    _objectBuffer.reserve(_objectSize);

    start_read_object();
}

void client::start_read_object() {
    _readState = ReadState::READING_OBJECT;

    _objectBuffer.resize(_objectSize);

    boost::asio::async_read(*_tcpSocket,
        boost::asio::buffer(&_objectBuffer[0], _objectSize),
        boost::asio::transfer_exactly(_objectSize),
        [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            std::cout << "[Client] Received object data (hex): ";
            for (size_t i = 0; i < bytes_transferred; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << (static_cast<unsigned int>(static_cast<unsigned char>(_objectBuffer[i])) & 0xFF) << " ";
            }
            std::cout << std::dec << std::endl;

            if (ec) {
                std::cerr << "[Client] Error reading object data: " << ec.message() << "\n";
                return;
            }

            auto factory = this->_game.getFactory();
            
            engine::Object *obj = engine::Object::deserializeFromBytes(_objectBuffer, factory);
            if (!obj) {
                std::cerr << "[Client] Failed to deserialize object\n";
                return;
            }
            obj->buildEntity(this->_game.getFactory());
            std::cout << "[Client] Received and deserialized object: " << obj->getName() << "\n";
            this->_game.loadObject(obj);

            start_read_until_delimiter();
        });
}

void client::start_read_until_delimiter() {
    _readState = ReadState::READING_UNTIL_DELIMITER;

    boost::asio::async_read_until(*_tcpSocket,
        boost::asio::dynamic_buffer(_objectBuffer),
        _delimiter,
        [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            if (ec) {
                if (ec == boost::asio::error::eof) {
                    std::cout << "[Client] Server closed the connection.\n";
                    _running = false;
                } else {
                    std::cerr << "[Client] Error reading until delimiter: " << ec.message() << "\n";
                }
                return;
            }

            std::cout << "[Client] Received delimiter data (hex): ";
            for (size_t i = bytes_transferred - _delimiter.size(); i < bytes_transferred; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << (static_cast<unsigned int>(static_cast<unsigned char>(_objectBuffer[i])) & 0xFF) << " ";
            }
            std::cout << std::dec << std::endl;

            std::string receivedData = _objectBuffer.substr(0, bytes_transferred - _delimiter.size());
            std::cout << "[Client] Received Data: " << receivedData << "\n";

            if (!receivedData.empty()) {
                auto factory = this->_game.getFactory();
                engine::Object* obj = engine::Object::deserializeFromBytes(receivedData, factory);
                if (!obj) {
                    std::cerr << "[Client] Failed to deserialize object\n";
                    return;
                }

                obj->buildEntity(this->_game.getFactory());
                std::cout << "[Client] Received and deserialized object: " << obj->getName() << "\n";
                this->_game.loadObject(obj);
            }

            _objectBuffer.erase(0, bytes_transferred);

            start_write_ack();
        });
}

void client::start_write_ack() {
    _readState = ReadState::SENDING_ACK;
    std::string ack = "ACK";
    boost::asio::async_write(*_tcpSocket,
        boost::asio::buffer(ack),
        [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            handle_write_ack(ec, bytes_transferred);
        });
}

void client::handle_write_ack(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    std::cout << "handle_write_ack called\n";
    if (ec) {
        std::cerr << "[Client] Failed to send acknowledgment: " << ec.message() << "\n";
        return;
    }
    std::cout << "[Client] Sent acknowledgment to server.\n";
    start_read_size();
}

void client::do_read() {
    start_read_size();
}

void client::update(void) {
    {
        std::unique_lock<std::mutex> lock(_loginMutex);
        _loginCondVar.wait(lock, [this] { return _loginCompleted; });
    }

    std::cout << "Before do_read/ioContext.poll()\n";

    this->_game.unloadAllObjects();
    do_read();

    std::cout << "After do_read/ioContext.poll()\n";

    _ioContext.poll();

    this->_displayManager.update();
}

void client::draw(void)
{
    this->_displayManager.clear();

    for (const auto &item : this->_game.getLoadedObjects()) {
        this->_displayManager.useEntity(*item->getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
    }

    for (const auto &item : this->_game.getLoadedHuds()) {
        this->_displayManager.useEntity(*item->getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
    }

    this->_orchestrator.callFunctionAll("post_draw");

    this->_displayManager.draw();
}

void client::login()
{
    if (!_tcpSocket || !_tcpSocket->is_open()) {
        std::cerr << "[Client] Cannot login, TCP socket not connected.\n";
        return;
    }

    std::string username = "test";
    std::vector<char> payload(username.begin(), username.end());

    std::uint16_t magic = 0xEB27;
    std::uint8_t  cmd = 0x01;
    std::uint16_t length = static_cast<std::uint16_t>(payload.size());

    std::vector<char> buffer(5 + length);

    std::memcpy(buffer.data(), &magic, 2);
    buffer[2] = static_cast<char>(cmd);
    std::memcpy(buffer.data() + 3, &length, 2);

    if (length > 0) {
        std::memcpy(buffer.data() + 5, payload.data(), length);
    }

    boost::system::error_code ec;
    std::size_t written = boost::asio::write(*_tcpSocket,
                                             boost::asio::buffer(buffer),
                                             ec);

    if (!ec) {
        std::cout << "[Client] Sent HELLO (username='test') -> " 
                  << written << " bytes\n";
    } else {
        std::cerr << "[Client] Send error: " << ec.message() << std::endl;
        return;
    }

    start_read_size();
}

void client::mainloop(void)
{
    this->login();
    this->_running = true;

    _ioThread = std::thread([this]() {
        _ioContext.run();
    });

    while (this->_running) {
        this->event();
        this->update();
        this->draw();
        this->_clock.tick(60);
    }

    _ioContext.stop();
    if (_ioThread.joinable()) {
        _ioThread.join();
    }
}
