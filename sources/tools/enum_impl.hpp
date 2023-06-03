#pragma once

#include "enum.hpp"

template< cEnum E >
std::istream & operator>> ( std::istream & in, E & value )
{
    int input;
    if ( in >> input )
    {
        if ( input >= 0 && static_cast< E >( input ) < LAST_ENUM( E ) )
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

template< cEnum E >
E to_enum ( std::string const & string )
{
    E value {};

    boost::mp11::mp_for_each< boost::describe::describe_enumerators< E > >(
        [&] ( auto D ) {
            if ( string == D.name )
            {
                value = D.value;
            }
        } );

    return value;
}

template< cEnum E >
std::string to_string ( E enumValue )
{
    std::string string { "" };

    boost::mp11::mp_for_each< boost::describe::describe_enumerators< E > >(
        [&] ( auto D ) {
            if ( D.value == enumValue )
            {
                string = D.name;
                return;
            }
        } );

    return string;
}

template< cEnum E >
std::vector< std::string > get_list ()
{
    std::vector< std::string > list {};

    boost::mp11::mp_for_each< boost::describe::describe_enumerators< E > >(
        [&] ( auto D ) {
            if ( D.name == LAST_ENUM_NAME_STR )
            {
                return;
            }
            list.push_back( D.name );
        } );

    return list;
}
