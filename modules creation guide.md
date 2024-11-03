<h1>Game Library Integration Technical Documentation</h1>

For this project of R-type, our program works thanks to diferrent modules: Network, Graphics, Clock, Game Core, and Components.
This doccument will explan how to make these modules for them to work with our programm.
**Table of Contents**

    Network Interface (INetwork)
    Graphics Interface (engine::displayManager)
    Clock Module (clock)
    Game Core (Game)
    Component Interface (IComponent)

1. **Network Interface**

The INetwork class defines a networking interface for communication between the players and the server. It should be able to send message, as well as encode them, decode and decode and receive message from the other composant.
<h2>Interface Definition</h2>


```c++

class INetwork {
public:
    virtual ~INetwork() {}

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port) = 0;
    virtual void setReceiveHandler(std::function<void(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)> handler) = 0;
    virtual void setClientConnectedHandler(std::function<void(const std::string& address, uint16_t port)> handler) = 0;
    virtual void setClientDisconnectedHandler(std::function<void(const std::string& address, uint16_t port)> handler) = 0;
};
```

<h2>Function Descriptions</h2>

    start(): Initializes and starts the network service.
    stop(): Shuts down the network service and disconnects all clients.
    send(): Transmits data to a specified address and port.
    setReceiveHandler(): Sets a callback function for handling incoming data.
    setClientConnectedHandler(): Sets a callback function for managing newly connected clients.
    setClientDisconnectedHandler(): Sets a callback function for managing client disconnections.

<h2>Notes for Implementation</h2>

    The network library should handle connection management autonomously after starting.
    Callback handlers should be invoked asynchronously to prevent blocking the game loop.

2. **Graphics Interface**

The engine::displayManager class provides an interface for graphical management, including asset handling, window management, and rendering.
It could be linked to any type of graphic library, 2D or 3D. It will just generate the display and receive the player events, but not do anything with those.
<h2>Interface Definition</h2>


```c++

namespace engine {
    class displayManager {
        public:
            displayManager();

            void registerAsset(const std::string &, const std::string &);
            void registerAsset(const std::string &, const std::shared_ptr<grw::texture> &);

            void addWindow(grw::window *);

            bool event(void);
            void update(void);
            void draw(void);
            void clear(void);

            unsigned int createWindow(void);

            void useEntity(const Entity &, Registry &, unsigned int);
        private:
            grw::windowHandler _winHandler;

            std::map<unsigned int, std::vector<grw::sprite>> _rendering;
            std::map<std::string, std::shared_ptr<grw::texture>> _textures;
    };
}
```

<h2>Function Descriptions</h2>

    registerAsset(): Loads assets either from a file path or as a texture object.
    addWindow(): Adds a new window to the rendering system.
    event(): Handles and processes window events.
    update(): Updates the display, managing any animations or transformations.
    draw(): Renders all current frames to the window.
    clear(): Clears the display before the next rendering cycle.
    createWindow(): Creates and registers a new window; returns an ID.
    useEntity(): Links entities to the display, utilizing the Entity and Registry structures.

<h2>Notes for Implementation</h2>

    The displayManager should support multiple windows and scenes for a flexible game environment.
    Implementations should handle frame management and synchronization to avoid visual glitches.
    Texture and sprite management should be memory-efficient.

3. **Clock Module**

The clock class provides a simple timing mechanism to manage game updates and frame timing. It will be implemented directly with the graphic module but also used by the program itself. The graphic will use it to be frame friendly and not have display lags, however, the game algorythm will work at the same speed even with diferrent frame settings.
<h2>Interface Definition</h2>


```c++

class clock {
    public:
        clock();

        unsigned long tick(int = -1);

    private:
        unsigned long _oldTime;
};
```

<h2>Function Descriptions</h2>

    tick(): Calculates and returns the elapsed time since the last call. If a parameter is provided, it sets the tick rate or time interval.

<h2>Notes for Implementation</h2>

    Implementations should ensure accurate timing for frame rate consistency.
    tick() should be lightweight to minimize performance overhead during game updates.

4. **Game Core**

The Game class serves as the primary game management core, handling scenes, objects, and game entities. Our version uses lua scripts and yaml scenes to handle everything, but it could be implemented directly in the game code if you want.
<h2>Interface Definition</h2>

```c++

class Game {
    public:
        Game();

        const Scene &getScene(const std::string &) const;
        const ObjectRef &getObject(const std::string &) const;

        void registerScene(const std::string &);
        void registerScene(const Scene &);

        void registerObject(const std::string &);
        void registerObject(const ObjectRef &);

        void unregisterScene(const std::string &);
        void unregisterObject(const std::string &);

        void loadObject(const ObjectRef &);

        void loadScene(const std::string &);
        void unloadScene(void);

        const std::vector<Object> &getLoadedObjects(void);
        const std::vector<Object> &getLoadedHuds(void);

        void addFactory(EntityFactory *);
        engine::EntityFactory *getFactory(void);

        Object buildObjectRef(const ObjectRef &, const std::string &);

    private:
        std::map<std::string, ObjectRef> _objects;
        std::map<std::string, Scene> _scenes;

        std::vector<Object> _loadedGameObjects;
        std::vector<Object> _loadedGameHuds;

        EntityFactory *_factory;

        std::string _loadedScene;
};
```

<h2>Function Descriptions</h2>

    registerScene() and registerObject(): Registers a scene or object by name or reference.
    unregisterScene() and unregisterObject(): Unregisters a scene or object.
    loadObject(): Loads an object into the game.
    loadScene() and unloadScene(): Loads or unloads a specific scene.
    getLoadedObjects() and getLoadedHuds(): Returns lists of currently loaded game objects or HUD elements.
    addFactory(): Adds a factory instance for creating entities.

<h2>Notes for Implementation</h2>

    Efficient memory management is critical, especially for large scenes and objects.

5. **Component Interface**

The IComponent class represents individual game components, which can be used to assemble complex entities.
Some examples can be size, position or sprite. It makes passing informations between the diferrent modules easier.
<h2>Interface Definition</h2>

```c++

class IComponent {
    public:
        IComponent() = default;
        virtual ~IComponent() = default;

        std::string name;
};
```

<h2>Notes for implementation</h2>

    Each component should have a name identifier for easy retrieval and management.

<h2>Conclusion</h2>

Following these specifications ensures compatibility with our R-type game system. Libraries implementing these interfaces should provide robust, efficient, and flexible functionality, allowing for seamless integration into our game engine. Make sure to adhere to the interface definitions and implement required functionalities precisely as described.