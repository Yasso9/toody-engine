#pragma once

#include "serialization.hpp"

#include "tools/assertion.hpp"
#include "tools/stream/stream.hpp"

template< typename Type >
Chunk serialize ( Type const & valueToSerialize )
{
    std::ostringstream stream {};
    stream << valueToSerialize;

    return Chunk { stream.str() };
}

/// @brief String Type
template< typename Type >
concept C_String = std::is_same_v< Type, std::string >
                   || std::is_same_v< Type, char const * >;

template< typename Type >
static Type unstream ( std::stringstream & stream )
{
    Type value;
    stream >> value;
    return value;
}

template< typename Type >
static Type unstream ( std::stringstream & stream )
    requires ( C_String< Type > )
{
    return stream.str();
}

template< typename Type >
Type Chunk::to_value() const
{
    std::stringstream stream {};
    stream << m_serializedValue;

    Type value { unstream< Type >( stream ) };

    ASSERTION( serialize( value ).to_string() == this->to_string(),
               "Unserialized value must be equal to the serialised value "
               "unserialised" );
    return value;
}
