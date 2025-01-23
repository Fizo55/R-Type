#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>
#include <yaml-cpp/yaml.h>

using namespace engine;

ObjectRef createPlayer()
{
    ObjectRef temp = ObjectRef();
    std::vector<std::any> coords;
    std::vector<std::any> size;
    std::vector<std::any> script;
    std::vector<std::any> sprite;

    coords.push_back(std::any((std::int64_t)100));
    coords.push_back(std::any((std::int64_t)110));

    size.push_back(std::any((std::int64_t)36));
    size.push_back(std::any((std::int64_t)18));

    script.push_back(std::any((std::string)"player_script"));
    sprite.push_back(std::any((std::string)"sprite_player1"));

    temp.addBuildParameter("coords", coords);
    temp.addBuildParameter("size", size);
    temp.addBuildParameter("sprite", sprite);
    temp.addBuildParameter("main_script", script);

    temp.setName("player");

    return (temp);
}


client::client(const std::string &configPath)
  : _running(false)
  , _tcpSocket(std::make_unique<boost::asio::ip::tcp::socket>(_ioContext))
{
    std::string serverIp = "127.0.0.1";
    unsigned short serverPort = 1337;

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
        this->_game.loadScene(gameData["entryScene"].as<std::string>());

    this->_gameWindow = this->_displayManager.createWindow(800, 600);

    this->_game.writeDBInt(0xF1, (std::int64_t)this->_gameWindow);
    this->_game.writeDBInt(0x00, (int64_t)this->_game.buildObjectRef(createPlayer(), "player0"));

    this->_game.loadObject((Object *)this->_game.readDBInt(0x00));

    this->_orchestrator.fromGameObject(this->_game);
}

client::~client()
{

}

void client::event(void)
{
    static bool up = false;
    static bool down = false;
    static bool left = false;
    static bool right = false;
    static bool space = false;

    this->_running = !this->_displayManager.event();

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_Z) {
            up = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_S) {
            down = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_Q) {
            left = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_D) {
            right = true;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_SPACE) {
            space = true;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_Z) {
            up = false;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_Q) {
            left = false;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_S) {
            down = false;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_D) {
            right = false;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_SPACE) {
            space = false;
    }

    if (up)
        this->_orchestrator.callFunctionAll("key_z");
    if (down)
        this->_orchestrator.callFunctionAll("key_s");
    if (left)
        this->_orchestrator.callFunctionAll("key_q");
    if (right) {
        this->_orchestrator.callFunctionAll("key_d");
    }
    if (space)
        this->_orchestrator.callFunctionAll("key_space");
}

void client::update(void)
{
    this->_orchestrator.callFunctionAll("update");

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

    std::uint16_t magic  = 0xEB27;
    std::uint8_t  cmd    = 0x01;
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
        std::cout << "[Client] Sent HELLO with hardcoded username 'test' ("
                  << written << " bytes)\n";
    } else {
        std::cerr << "[Client] Send error: " << ec.message() << std::endl;
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
