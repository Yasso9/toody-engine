#pragma once

#include "tools/string.hpp"

#include <iostream>

#ifndef NDEBUG
    #define Assertion( expression, message ) \
        too::assertion( expression, \
                        __FILE__, \
                        __LINE__, \
                        __func__, \
                        #expression, \
                        message )
#else
    #define Assertion( expression, message ) ;
#endif

namespace too
{
    // TYPO tester si ça marche bien
    void assertion( bool const & expression, std::string const & fileName,
                    int const & line, std::string const & functionName,
                    std::string const & expressionString,
                    std::string const & message = ""s );
} // namespace too