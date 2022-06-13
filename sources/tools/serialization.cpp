#include "serialization.hpp"

#include <sstream>

bool verify_next( std::istream & stream, char const & character )
{
    char charToRead {};
    stream >> charToRead;

    return character == charToRead;
}

void test()
{
    std::vector< std::vector< int > > array {
        {745,   524,  879,   858},
        {745,   454,  879, 45714},
        {745, 45474, 4141,   858},
        {474,   454,  879,   858}
    };
    std::cout << "array : " << array << std::endl;

    std::stringstream stream {};
    stream << array;
    std::cout << "stream :" << stream.str() << std::endl;

    std::vector< int > newArray {};
    stream >> newArray;
    std::cout << "newArray : " << array << std::endl;
}