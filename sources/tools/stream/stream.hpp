#pragma once

#include <istream>
#include <vector>

#include "tools/concepts.hpp"

namespace stream
{
    // If the next character is character, it is ignored int the stream
    bool ignore_next ( std::istream & stream, char character );
    // TODO this function must have a const parameter
    char peek_next ( std::istream & stream );

    bool is_ended ( std::istream const & stream );
}  // namespace stream

/// @brief Reading vector
template< typename Type >
std::ostream & operator<< ( std::ostream &              stream,
                            std::vector< Type > const & array );

template< typename Type >
std::istream & operator>> ( std::istream &        stream,
                            std::vector< Type > & array );

#include "stream.tpp"
