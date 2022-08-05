#pragma once

// Put this line in all derived class :
// friend DerivedClass & Singleton< DerivedClass >::get_instance();

/// @todo put a concept to force the fact that DerivedClass is a class or a
/// struct, and not an other thing
template< typename SingletonClass >
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