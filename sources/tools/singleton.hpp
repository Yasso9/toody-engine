#pragma once

// Put this line in all derived class :
// friend DerivedClass & Singleton< DerivedClass >::get_instance();

template< typename SingletonClass >
    requires( std::is_class_v< SingletonClass > )

class Singleton
{
  public:
    Singleton( Singleton const & )       = delete;
    void operator= ( Singleton const & ) = delete;

    static SingletonClass & get_instance ();

  protected:
    Singleton()          = default;
    virtual ~Singleton() = default;
};

#include "singleton.tpp"