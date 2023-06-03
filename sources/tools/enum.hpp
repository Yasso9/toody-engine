#pragma once

#include <istream>
#include <ostream>
#include <type_traits>
#include <vector>

#include <boost/describe.hpp>  // for BOOST_DESCRIBE_NESTED_ENUM

#define LAST_ENUM_NAME     EnumLast
#define LAST_ENUM_NAME_STR "EnumLast"
#define LAST_ENUM( Enum )  Enum::LAST_ENUM_NAME

template< typename T > concept cEnum = std::is_enum_v< T >;

template< cEnum E >
std::istream & operator>> ( std::istream & in, E & value );
template< cEnum E >
std::ostream & operator<< ( std::ostream & out, E const & value );

template< cEnum E >
E to_enum ( std::string const & string );
template< cEnum E >
std::string to_string ( E enumValue );
template< cEnum E >
std::vector< std::string > get_list ();

#include "enum_impl.hpp"
