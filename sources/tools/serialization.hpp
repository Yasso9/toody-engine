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

class Chunk
{
    std::string m_serializedValue;

  public:
    Chunk( std::string const & serializedValue );

    /// @brief return a string of the serialized content
    std::string to_string () const;

    template< typename Type >
    Type to_value () const;
};

/// @todo Serializer and Unserializer must be the same class
/// @todo Type require that the type have operator << and copy contructor
template< typename Type >
Chunk serialize ( Type const & valueToSerialize );