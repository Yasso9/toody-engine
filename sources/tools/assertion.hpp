#pragma once

#include "tools/string.hpp"

#include <iostream>

#ifndef NDEBUG

    #define ASSERTION2( expression, message ) \
        too::assertion( expression,           \
                        __FILE__,             \
                        __LINE__,             \
                        __func__,             \
                        #expression,          \
                        message )

    #define ASSERTION1( expression )           ASSERTION2( expression, "" )

    #define GET_MACRO( _1, _2, NAME, ... ) NAME
    #define ASSERTION( ... ) \
        GET_MACRO( __VA_ARGS__, ASSERTION2, ASSERTION1 )( __VA_ARGS__ )
#else
    #define ASSERTION( expression, message ) ;
    #define ASSERTION( expression )          ;
#endif

namespace too
{
    void assertion( bool const & expression, std::string const & fileName,
                    int const & line, std::string const & functionName,
                    std::string const & expressionString,
                    std::string const & message = ""s );
} // namespace too