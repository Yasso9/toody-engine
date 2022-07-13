#include "serialization.hpp"

#include <sstream>

bool verify_next( std::istream & stream, char const & character )
{
    if ( stream.peek() != character )
    {
        std::stringstream streamError {};
        streamError << "We should have the character : '" << character
                    << "' and not '" << stream.peek() << "'\n";
        throw std::runtime_error { streamError.str() };

        return false;
    }

    // Advence of one character
    stream.seekg( 1 + stream.tellg() );

    return true;
}

void test_serializer()
{
    std::vector< int > arrayA { 714545, 4524, 9, 10002 };
    std::cout << "arrayA : " << arrayA << std::endl;

    std::stringstream streamA {};
    streamA << arrayA;
    std::cout << "streamA : " << streamA.str() << std::endl;

    std::vector< int > newarrayA {};
    streamA >> newarrayA;
    std::cout << "newarrayA : " << newarrayA << std::endl;

    std::cout << "\n\n\n" << std::endl;

    std::vector< std::vector< int > > arrayB {
        {745,  524,   879,  858  },
        { 745, 454,   879,  45714},
        { 745, 45474, 4141, 858  },
        { 474, 454,   879,  858  }
    };
    std::cout << "arrayB : " << arrayB << std::endl;

    std::stringstream streamB {};
    streamB << arrayB;
    std::cout << "streamB :" << streamB.str() << std::endl;

    std::vector< std::vector< int > > newarrayB {};
    streamB >> newarrayB;
    std::cout << "newarrayB : " << arrayB << std::endl;
}