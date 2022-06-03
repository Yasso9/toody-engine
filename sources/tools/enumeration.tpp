#pragma once

#include "tools/enumeration.hpp"

template < EnumType Type >
Enum< Type >::Enum( Type const & enumValue ) : m_currentValue( enumValue )
{}

template < EnumType Type >
Enum< Type > & Enum< Type >::operator++()
{
    // Convert the enum value to int
    int integerValue { static_cast< int >( this->m_currentValue ) };
    // Know we can increment it
    ++integerValue;
    // Revert integer to enum
    this->m_currentValue = static_cast< Type >( integerValue );
    return *this;
}

// postfix increment
template < EnumType Type >
Enum< Type > Enum< Type >::operator++( int )
{
    Enum< Type > const oldEnum { *this };
    this->operator++();
    return oldEnum;
}

template < EnumType Type >
bool Enum< Type >::operator<( Type const & enumValue )
{
    return this->m_currentValue < enumValue;
}
template < EnumType Type >
bool Enum< Type >::operator>( Type const & enumValue )
{
    return this->m_currentValue > enumValue;
}

template < EnumType Type >
Type Enum< Type >::get_min()
{
    return static_cast< Type >( 0 );
}

template < EnumType Type >
Type Enum< Type >::get_max()
{
    return Type::EnumLast;
}

template < EnumType Type >
Type Enum< Type >::get_value() const
{
    return this->m_currentValue;
}

template < EnumType EnumType >
std::ostream & operator<<( std::ostream & output,
                           Enum< EnumType > const & enumValue )
{
    output << static_cast< int >( enumValue.get_value() );
    return output;
}