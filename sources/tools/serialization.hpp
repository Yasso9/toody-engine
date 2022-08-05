#pragma once

#include <sstream>  // for istream, ostream
#include <string>   // for string
#include <vector>   // for vector

/// @todo differences between typename or class in template ?
/// @todo Type require that the type have operator <<
template< typename Type >
std::ostream & operator<< (
    std::ostream & stream, std::vector< Type > const & array );

template< typename Type >
std::istream & operator>> (
    std::istream & stream, std::vector< Type > & array );

/// @todo improve this function either by taking into account the return value
/// or launch an exception or an assertion
bool verify_next ( std::istream & stream, char const & character );

[[maybe_unused]] void test_serializer ();

/// @todo Serializer and Unserializer must be the same class

/// @todo Type require that the type have operator << and copy contructor
template< typename TypeToSerialize >
class Serializer
{
    TypeToSerialize m_valueToSerialize;

  public:
    Serializer( TypeToSerialize const & valueToSerialize );

    std::string to_string () const;
};

class Unserializer
{
    std::string m_stringToUnserialize;

  public:
    Unserializer( std::string const & stringToUnserialize );

    /// @brief return the serialized content
    std::string get_content () const;

    template< typename TypeToUnserialize >
    TypeToUnserialize to_value () const;
};
