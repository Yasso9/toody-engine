#pragma once

#include "singleton.hpp"

template < typename DerivedClass >
DerivedClass & Singleton< DerivedClass >::get_instance()
{
    static DerivedClass instance {};
    return instance;
}