#include "serialization.hpp"

#include <iostream>   // for cout
#include <stdexcept>  // for runtime_error

#include "tools/serialization.tpp"  // for operator<<, operator>>

Chunk::Chunk( std::string const & stringToUnserialize )
  : m_serializedValue { stringToUnserialize }
{}

std::string Chunk::to_string() const
{
    return m_serializedValue;
}

void test_serializer ()
{
    std::vector< int > arrayA { 714545, 4524, 9, 10002 };
    std::cout << "arrayA : " << arrayA << std::endl;

    std::stringstream streamA {};
    streamA << arrayA;
    std::cout << "streamA : " << streamA.str() << std::endl;

    std::vector< int > newarrayA {};
    streamA >> newarrayA;
    std::cout << "newarrayA : " << newarrayA << std::endl;

    std::cout << "\n" << std::endl;

    std::vector< std::vector< int > > arrayB { { 745, 524, 879, 858 },
                                               { 745, 454, 879, 45714 },
                                               { 745, 45474, 4141, 858 },
                                               { 474, 454, 879, 858 } };
    std::cout << "arrayB : " << arrayB << std::endl;

    std::stringstream streamB {};
    streamB << arrayB;
    std::cout << "streamB :" << streamB.str() << std::endl;

    std::vector< std::vector< int > > newarrayB {};
    streamB >> newarrayB;
    std::cout << "newarrayB : " << arrayB << std::endl;
}