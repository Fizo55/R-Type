#include "MessageBase.hpp"

MessageBase::MessageBase(uint32_t type)
    : type_(type)
{
}

uint32_t MessageBase::getType() const
{
    return type_;
}
