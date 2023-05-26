#pragma once

#include "enum.hpp"

template< cEnum E >
std::istream & operator>> ( std::istream & in, E & value )
{
    int input;
    if ( in >> input )
    {
        if ( input >= 0 && input < E::EnumLast )
        {
            value = static_cast< E >( input );
        }
        else
        {
            in.setstate( std::ios::failbit );
        }
    }
    return in;
}

template< cEnum E >
std::ostream & operator<< ( std::ostream & out, E const & value )
{
    out << static_cast< int >( value );
    return out;
}
