#pragma once
#include "LoggedUser.h"
#include <string>

std::string LoggedUser::getUsername() const
{
    return m_userName;
}
    