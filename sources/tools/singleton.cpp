#include "singleton.hpp"

Singleton & Singleton::get_instance()
{
    static Singleton instance;
    return instance;
}