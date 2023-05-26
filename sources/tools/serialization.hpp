#pragma once

#include <sstream>  // for istream, ostream
#include <string>   // for string
#include <vector>   // for vector

#include "tools/stream/stream.hpp"

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

template< typename Type >
Chunk serialize ( Type const & valueToSerialize );
