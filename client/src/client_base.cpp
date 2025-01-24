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

bool receiveWithSizeHeader(boost::asio::ip::tcp::socket& socket, std::string& data)
{
    char sizeBuffer[2];
    boost::system::error_code ec;

    size_t bytesRead = boost::asio::read(
        socket,
        boost::asio::buffer(sizeBuffer, 2),
        boost::asio::transfer_exactly(2),
        ec
    );

    if (ec) {
        std::cerr << "[Client] Failed to read size header: " << ec.message() << "\n";
        return false;
    }

    uint16_t size = 0;
    std::memcpy(&size, sizeBuffer, sizeof(size));
    size = ntohs(size);

    std::vector<char> payloadBuffer(size);
    bytesRead = boost::asio::read(
        socket,
        boost::asio::buffer(payloadBuffer),
        boost::asio::transfer_exactly(size),
        ec
    );

    if (ec) {
        std::cerr << "[Client] Failed to read JSON payload: " << ec.message() << "\n";
        return false;
    }

    data.assign(payloadBuffer.begin(), payloadBuffer.end());
    return true;
}

void client::update(void)
{
    {
        std::unique_lock<std::mutex> lock(_loginMutex);
        _loginCondVar.wait(lock, [this] { return _loginCompleted; });
    }

    this->_game.unloadAllObjects();

     try {
        while (true) {
            std::string jsonString;
            if (!receiveWithSizeHeader(*_tcpSocket, jsonString)) {
                break;
            }

            std::cout << "[Client] Received JSON: " << jsonString << "\n";

            try {
                std::unique_ptr<engine::Object> obj = engine::Object::deserializeFromJson(jsonString);
                if (obj) {
                    obj->buildEntity(this->_game.getFactory());

                    std::cout << "[Client] Deserialized Object: " << obj->getName() << "\n";

                    this->_game.loadObject(obj.get());
                }
                else {
                    std::cerr << "[Client] Failed to deserialize object.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "[Client] Deserialization exception: " << e.what() << "\n";
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << "[Client] Exception during update: " << e.what() << "\n";
    }

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

    {
        char idBuffer[64];
        boost::system::error_code readEc;

        std::size_t bytesRead = boost::asio::read(
            *_tcpSocket,
            boost::asio::buffer(idBuffer),
            boost::asio::transfer_at_least(1),
            readEc
        );

        if (!readEc) {
            std::string sessionID(idBuffer, bytesRead);
            std::cout << "[Client] Received session ID from server: " 
                    << sessionID << std::endl;
            this->_game.writeDBInt(0x00, (std::int64_t)std::stoul(sessionID));
            _loginCompleted = true;
        } else {
            std::cerr << "[Client] Read error: " << readEc.message() << std::endl;
        }
    }
}

void client::mainloop(void)
{
    this->login();
    this->_running = true;

    while (this->_running) {
        this->event();
        this->update();
        this->draw();
        this->_clock.tick(60);
    }

}
