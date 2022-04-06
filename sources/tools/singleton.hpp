#pragma once

template < typename DerivedClass >
class Singleton
{
  public:
    Singleton( Singleton const & ) = delete;
    void operator=( Singleton const & ) = delete;

    virtual ~Singleton() = default;

    static DerivedClass & get_instance();

  protected:
    Singleton() = default;
};

#include "singleton.tpp"