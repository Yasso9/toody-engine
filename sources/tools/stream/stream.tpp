#include "stream.hpp"

#include <sstream>

template< typename Type >
std::ostream & operator<< ( std::ostream &              stream,
                            std::vector< Type > const & array )
{
    stream << '[';

    if ( ! array.empty() )
    {
        stream << ' ';

        for ( Type const & element : array )
        {
            stream << element;
            stream << ' ';
        }
    }

    stream << ']';

    return stream;
}

template< typename Type >
std::istream & operator>> ( std::istream & stream, std::vector< Type > & array )
{
    array = std::vector< Type > {};

    stream::ignore_next( stream, '[' );
    while ( ! stream.fail() && stream::peek_next( stream ) != ']' )
    {
        Type element;
        stream >> element;

        array.push_back( element );
    }
    stream::ignore_next( stream, ']' );

    return stream;
}