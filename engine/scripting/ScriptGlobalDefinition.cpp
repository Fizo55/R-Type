#include "engineScripting.hpp"

engine::ScriptGlobalDefinition::ScriptGlobalDefinition(IScriptTypeDefinitor *type, const std::string &luaTable, const std::string &name, void *data)
  : type(type), luaTable(luaTable), name(name), data(data)
{

}
