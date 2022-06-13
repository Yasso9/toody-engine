#include "serialization.hpp"

bool verify_next( std::istream & stream, char const & character )
{
    char charToRead {};
    stream >> charToRead;

    return character == charToRead;
}