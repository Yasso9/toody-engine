#pragma once

#include "serialization.hpp"
#include "tools/print.hpp"

template < typename Type >
std::ostream & operator<<( std::ostream & stream,
                           std::vector< Type > const & array )
{
    stream << '[';
    stream << ' ';
    for ( Type const & element : array )
    {
        stream << element;
        stream << ' ';
    }
    stream << ']';

    return stream;
}

template < typename Type >
std::istream & operator>>( std::istream & stream, std::vector< Type > & array )
{
    array = std::vector< Type > {};

    verify_next( stream, '[' );
    verify_next( stream, ' ' );
    while ( ! stream.fail() && stream.peek() != ']' && stream.peek() != EOF )
    {
        Type element;
        stream >> element;

        array.push_back( element );

        verify_next( stream, ' ' );
    }
    verify_next( stream, ']' );

    return stream;
}

template < typename TypeToSerialize >
Serializer< TypeToSerialize >::Serializer(
    TypeToSerialize const & valueToSerialize )
  : m_valueToSerialize( valueToSerialize )
{}

template < typename TypeToSerialize >
std::string Serializer< TypeToSerialize >::to_string() const
{
    std::ostringstream stream {};
    stream << this->m_valueToSerialize;
    return stream.str();
}

template < typename TypeToUnserialize >
TypeToUnserialize Unserializer::to_value() const
{
    std::stringstream stream {};
    stream << this->m_stringToUnserialize;

    TypeToUnserialize value {};
    stream >> value;
    return value;

    /// @todo assert that the unserialized is equal to the value serialize
}
