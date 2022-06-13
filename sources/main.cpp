// #include "main/game.hpp"

#include <sstream>
#include "tools/serialization.hpp"

int main()
{
    // Game game {};

    // game.run();

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

    return EXIT_SUCCESS;
}