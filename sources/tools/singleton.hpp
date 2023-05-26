#pragma once

template< typename T >
requires ( std::is_class_v< T > )
class Singleton
{
  public:
    // A singleton should not be copyable or movable
    Singleton( Singleton const & )       = delete;
    void operator= ( Singleton const & ) = delete;
    Singleton( Singleton && )            = delete;
    void operator= ( Singleton && )      = delete;

    static T & get_instance ();

  protected:
    Singleton()          = default;
    virtual ~Singleton() = default;
};

// Use this macro inside the class that inherit from Singleton to allow
// get_instance() to access the constructor
#define ENABLE_SINGLETON( Type ) friend Type & Singleton< Type >::get_instance()

#include "singleton.tpp"
