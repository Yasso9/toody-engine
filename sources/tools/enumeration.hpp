#pragma once

#include <ostream>

template < typename E >
concept EnumType = std::is_enum_v< E >;

/// @brief When needed to do operation on enum
template < EnumType Type >
class Enum
{
  public:
    Enum( Type const & enumValue );
    virtual ~Enum() = default;

    // Increment and decrement operator
    Enum & operator++();
    Enum operator++( int );
    Enum & operator--();
    Enum operator--( int );

    bool operator<( Type const & enumValue );
    bool operator>( Type const & enumValue );

    static Type get_min();
    static Type get_max();

    Type get_value() const;

  protected:
    Type m_currentValue;
};

template < EnumType EnumType >
std::ostream & operator<<( std::ostream & output,
                           Enum< EnumType > const & enumValue );

#include "tools/enumeration.tpp"