#include "engineCore.hpp"

engine::ObjectRef::ObjectRef()
    : _name("new object")
{

}

engine::ObjectRef::ObjectRef(const std::string &path)
{

}

engine::Object::Object()
    : ObjectRef() {

}

engine::Object::Object(const std::string &path)
    : ObjectRef(path) {

}
