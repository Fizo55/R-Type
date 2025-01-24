#include "gameServer.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>
#include <yaml-cpp/yaml.h>

using namespace engine;

ObjectRef createPlayer(void)
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

server::server(const std::string &configPath)
 : _running(false)
{
    engine::ScriptTypeDefinitor<Game> *gameDefinitor = new engine::ScriptTypeDefinitor<Game>();
    engine::ScriptTypeDefinitor<grw::clock> *clockDefinitor = new engine::ScriptTypeDefinitor<grw::clock>();

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
    this->_orchestrator.addBinding(clock_register);

    this->_game.addFactory(&this->_factory);

    if (gameData["entryScene"])
        this->_game.loadScene(gameData["entryScene"].as<std::string>());

    // this->_game.writeDBInt(0x00, (int64_t)this->_game.buildObjectRef(createPlayer(), "player" + std::to_string(userId)));

    // this->_game.loadObject((Object *)this->_game.readDBInt(0x00));

    this->_orchestrator.fromGameObject(this->_game);
}

server::~server()
{
    _server->stop();

    _io.stop();
    if (_ioThread.joinable()) {
        _ioThread.join();
    }
}

void server::event(void)
{

}

void server::update(void)
{
    this->_orchestrator.callFunctionAll("update");
}

int server::listen(const std::string &ipAddress, uint16_t port)
{
    boost::system::error_code ec;
    boost::asio::ip::make_address(ipAddress, ec);
    if (ec) {
        std::cerr << "Invalid IP address: " << ipAddress << "\n";
        return (1);
    }

    std::cout << "[Main] Starting server on IP=" << ipAddress 
              << " TCP=" << port << " UDP=" << (port+1) << "\n";

    this->_server = std::make_shared<NetworkServer>(_io, ipAddress, port, port + 1);
    this->_server->start();

    _ioThread = std::thread([&]() {
        this->_io.run();
    });

    return (0);
}

void server::mainloop(void)
{
    this->_running = true;

    while (this->_running) {
        this->event();
        this->update();
        this->_clock.tick(60);
    }
}
