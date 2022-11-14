#pragma once

#include <ostream>

class Printable
{
  public:
    virtual ~Printable() = default;

  protected:
    virtual void print_to_stream ( std::ostream & stream ) const = 0;

    friend std::ostream & operator<< (
        std::ostream & stream, Printable const & printableObject );
};