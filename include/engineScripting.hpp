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
                ScriptEnvironment();
                ~ScriptEnvironment();

                void buildCoreLibrary();

                void loadScript(const std::string &);

                void callFunction(const std::string &);

                void registerGlobalObject(const ScriptGlobalDefinition &scriptDef)
                {
                    scriptDef.type->magicSet(lua_newuserdata(this->_ctx, scriptDef.type->magicSize()), scriptDef.data);
                    luaL_setmetatable(this->_ctx, scriptDef.luaTable.c_str());
                    lua_setglobal(this->_ctx, scriptDef.name.c_str());
                }
                lua_State *getCtx(void) const;
            private:
                lua_State *_ctx;
        };

        class ScriptOrchestrator {
            public:
                ScriptOrchestrator();
                ~ScriptOrchestrator() = default;

                void registerScript(const std::string &, const std::string &);
                void buildScript(const std::string &name, const std::vector<engine::ScriptGlobalDefinition> &extraDefs = std::vector<engine::ScriptGlobalDefinition>());

                void registerGlobal(const ScriptGlobalDefinition &scriptDef)
                {
                    this->_luaGlobals.push_back(scriptDef);
                }

                void fromGameObject(Game &);
                void addBinding(std::function<void(lua_State *)>);
                void callFunctionAll(const std::string &name);

            private:
                std::map<std::string, std::shared_ptr<ScriptEnvironment>> _scripts;
                std::map<std::string, std::string> _registeredScripts;
                std::vector<ScriptGlobalDefinition> _luaGlobals;
                std::vector<std::function<void(lua_State *)>> _bindings;
        };
    };

    void game_register(lua_State *ctx);
    void object_register(lua_State *ctx);

#endif /* !ENGINE_SCRIPTING_HPP_ */
