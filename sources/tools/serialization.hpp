#pragma once

#include <iostream>
#include <sstream>
#include <vector>

/// @todo differences between typename or class in template ?
/// @todo Type require that the type have operator <<
/// @todo std::instead of std::vector choose a collection type thatcan handle only array, vector, stack, etc ...
template < typename Type >
std::ostream & operator<<( std::ostream & stream,
                           std::vector< Type > const & array );

template < typename Type >
std::istream & operator>>( std::istream & stream, std::vector< Type > & array );

/// @todo improve this function either by taking into account the return value or launch an exception or an assertion
bool verify_next( std::istream & stream, char const & character );

[[maybe_unused]] void test_serializer();

/// @todo Type require that the type have operator << and copy contructor
template < typename TypeToSerialize >
class Serializer
{
    TypeToSerialize m_valueToSerialize;

  public:
    Serializer( TypeToSerialize const & valueToSerialize )
      : m_valueToSerialize( valueToSerialize )
    {}

    std::string to_string() const
    {
        std::ostringstream stream {};
        stream << this->m_valueToSerialize;
        return stream.str();
    }
};

class Unserializer
{
    std::string m_stringToUnserialize;

  public:
    Unserializer( std::string const & stringToUnserialize )
      : m_stringToUnserialize( stringToUnserialize )
    {}

    /// @brief return the serialized content
    std::string get_content() const { return m_stringToUnserialize; }

    template < typename TypeToUnserialize >
    TypeToUnserialize to_value() const
    {
        std::stringstream stream {};
        stream << this->m_stringToUnserialize;

        TypeToUnserialize value {};
        stream >> value;
        return value;

        /// @todo assert that the unserialized is equal to the value serialize
    }
};

#include "serialization.tpp"