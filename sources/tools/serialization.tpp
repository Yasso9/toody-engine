#include "serialization.hpp"

#include "tools/string.hpp"

template < typename Type >
std::ostream & operator<<( std::ostream & stream,
                           std::vector< Type > const & array )
{
    stream << '[';
    stream << ' ';
    for ( Type const & element : array )
    {
        stream << element;
        stream << ' ';
    }
    stream << ']';

    return stream;
}

template < typename Type >
std::istream & operator>>( std::istream & stream, std::vector< Type > & array )
{
    array = std::vector< Type > {};

    verify_next( stream, '[' );
    verify_next( stream, ' ' );
    while ( ! stream.fail() && stream.peek() != ']' && stream.peek() != EOF )
    {
        Type element;
        stream >> element;

        array.push_back( element );

        verify_next( stream, ' ' );
    }
    verify_next( stream, ']' );

    return stream;
}
