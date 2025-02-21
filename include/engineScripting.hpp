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
    #include <variant>

    #include "engineCore.hpp"

    namespace engine {
        class IScriptTypeDefinitor {
            public:
                IScriptTypeDefinitor() = default;
                ~IScriptTypeDefinitor() = default;

                virtual void magicSet(void *, void *) const = 0;
                virtual std::size_t magicSize(void) const = 0;
        };

        template <typename T>
        class ScriptTypeDefinitor : public IScriptTypeDefinitor {
            public:
                ScriptTypeDefinitor() = default;
                ~ScriptTypeDefinitor() = default;

                void magicSet(void *data, void *other) const {
                    *reinterpret_cast<T**>(data) = (T*)other;
                }
                std::size_t magicSize(void) const
                {
                    return (sizeof(T*));
                }
        };

        class ScriptGlobalDefinition {
            public:
                ScriptGlobalDefinition(IScriptTypeDefinitor *, const std::string &, const std::string &, void *);
                ~ScriptGlobalDefinition() = default;

                IScriptTypeDefinitor *type;
                std::string name;
                std::string luaTable;
                void *data;
        };

        class ScriptEnvironment {
            public:
                ScriptEnvironment(std::size_t ep = SIZE_MAX);
                ~ScriptEnvironment();

                void buildCoreLibrary();

                void loadScript(const std::string &);

                void callFunction(const std::string &);

                void registerGlobalObject(const ScriptGlobalDefinition &);
                lua_State *getCtx(void) const;

                void stopUsing(void);

                bool doStopUsing;

                std::size_t ep;
            private:
                lua_State *_ctx;
        };

        class ScriptOrchestrator {
            public:
                ScriptOrchestrator();
                ~ScriptOrchestrator();

                void registerScript(const std::string &, const std::string &);

                void buildScript(const std::string &name, const std::vector<engine::ScriptGlobalDefinition> &extraDefs = std::vector<engine::ScriptGlobalDefinition>(), std::size_t ep = SIZE_MAX);

                void registerGlobal(const ScriptGlobalDefinition &scriptDef)
                {
                    this->_luaGlobals.push_back(scriptDef);
                }

                void clearScripts(void);

                void fromGameObject(Game &);
                void fromObject(engine::EntityFactory *, Object *);
                void addBinding(std::function<void(lua_State *)>);
                void callFunctionAll(const std::string &name);

                void removeScript(std::size_t ep);

            private:
                std::vector<std::shared_ptr<ScriptEnvironment>> _scripts;
                std::map<std::size_t, std::shared_ptr<std::vector<engine::ScriptGlobalDefinition>>> _tempRegister;
                std::map<std::string, std::string> _registeredScripts;
                std::vector<ScriptGlobalDefinition> _luaGlobals;
                std::vector<std::function<void(lua_State *)>> _bindings;
        };
    };

    void game_register(lua_State *ctx);
    void object_register(lua_State *ctx);
    void orchestrator_register(lua_State *ctx);
    void any_list_register(lua_State *ctx);
    void clock_register(lua_State *ctx);
    void display_manager_register(lua_State *ctx);

#endif /* !ENGINE_SCRIPTING_HPP_ */
