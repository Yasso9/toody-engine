#pragma once

// Put this line in all derived class :
// friend DerivedClass & Singleton< DerivedClass >::get_instance();

template < typename DerivedClass >
class Singleton
{
  public:
    Singleton( Singleton const & ) = delete;
    void operator=( Singleton const & ) = delete;

    static DerivedClass & get_instance();

  protected:
    Singleton()          = default;
    virtual ~Singleton() = default;
};

#include "singleton.tpp"