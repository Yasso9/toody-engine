#include "enumeration.hpp"

template <typename Type>
Enum<Type>::Enum( Type const & enumValue ) : m_currentValue( enumValue )
{
}

template <typename Type>
Enum<Type> & Enum<Type>::operator++()
{
    // Convert the enum value to int
    int integerValue { static_cast<int>( this->m_currentValue ) };
    // Know we can increment it
    ++integerValue;
    // Revert integer to enum
    this->m_currentValue = static_cast<Type>( integerValue );
    return *this;
}

// postfix increment
template <typename Type>
Enum<Type> Enum<Type>::operator++( int )
{
    Enum<Type> const oldEnum { *this };
    this->operator++();
    return oldEnum;
}

template <typename Type>
bool Enum<Type>::operator<( Type const & enumValue ) noexcept
{
    return this->m_currentValue < enumValue;
}
template <typename Type>
bool Enum<Type>::operator>( Type const & enumValue ) noexcept
{
    return this->m_currentValue > enumValue;
}

template <typename Type>
Type Enum<Type>::get_min() noexcept
{
    return static_cast<Type>( 0 );
}

template <typename Type>
Type Enum<Type>::get_max() noexcept
{
    return Type::EnumLast;
}

template <typename Type>
Type Enum<Type>::get_value() const noexcept
{
    return this->m_currentValue;
}

// template class Enum<PlayerState>;
// template class Enum<Direction>;
// template class Enum<StateName>;
template class Enum<TextureKey>;
template class Enum<FontKey>;

// // prefix decrement
// Enum & Enum::operator--()
// {
//     // actual decrement takes place here
//     return *this; // return new enumValue by reference
// }

// // postfix decrement
// Enum Enum::operator--( int )
// {
//     X old = *this; // copy old enumValue
//     operator--(); // prefix decrement
//     return old; // return old enumValue
// }