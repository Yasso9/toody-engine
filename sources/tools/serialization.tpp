#include "serialization.hpp"

template < typename Type >
std::ostream & operator<<( std::ostream & stream,
                           std::vector< Type > const & array )
{
    stream << "[ ";
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
    array.clear();

    verify_next( stream, '[' );
    verify_next( stream, ' ' );

    while ( stream )
    {
        Type element;
        stream >> element;
        array.push_back( element );

        verify_next( stream, ' ' );
    }
    verify_next( stream, ']' );

    return stream;
}
