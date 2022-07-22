#pragma once

#include "singleton.hpp"

template < typename SingletonClass >
SingletonClass & Singleton< SingletonClass >::get_instance()
{
    static SingletonClass instance {};
    return instance;
}