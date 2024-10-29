#ifndef ENGINE_SCRIPTING_HPP_
    #define ENGINE_SCRIPTING_HPP_

    #ifdef __cplusplus
        extern "C" {
            #include <lua.h>
            #include <lualib.h>
            #include <lauxlib.h>
        }
    #endif //__cplusplus

    #include <iostream>

    #include "engineCore.hpp"

    namespace engine {
        class ScriptEnvironment {
            public:
                ScriptEnvironment();
                ~ScriptEnvironment();

                void buildCoreLibrary();

                void loadScript(const std::string &);

            private:
                lua_State *_ctx;
        };

        class ScriptOrchestrator {
            public:
                ScriptOrchestrator();
                ~ScriptOrchestrator() = default;

                void registerScript(const std::string &, const std::string &);
                void buildScript(const std::string &name);
                void fromGameObject(Game &);
            private:
                std::map<std::string, std::shared_ptr<ScriptEnvironment>> _scripts;
                std::map<std::string, std::string> _registeredScripts;
        };
    };

    void game_register(lua_State *ctx);

#endif /* !ENGINE_SCRIPTING_HPP_ */
