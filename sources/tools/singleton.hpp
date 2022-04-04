#pragma once

class Singleton
{
  public:
    Singleton( Singleton const & )      = delete;
    void operator=( Singleton const & ) = delete;

    static Singleton & get_instance();

  private:
    Singleton() = default;
};