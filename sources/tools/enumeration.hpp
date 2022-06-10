#pragma once

#include <ostream>

template < typename E >
concept EnumType = std::is_enum_v< E >;

/// @brief When needed to do operation on enum
template < EnumType Type >
class Enum
{
  protected:
    Type m_currentValue;

  public:
    static Type get_min();
    static Type get_max();

    Enum( Type const & enumValue );
    virtual ~Enum() = default;

    Type get_value() const;

    // Increment and decrement operator
    Enum & operator++();
    Enum operator++( int );
    Enum & operator--();
    Enum operator--( int );

    bool operator<( Type const & enumValue );
    bool operator>( Type const & enumValue );
};

template < EnumType EnumType >
std::ostream & operator<<( std::ostream & output,
                           Enum< EnumType > const & enumValue );

#include "tools/enumeration.tpp"