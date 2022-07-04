#pragma once

#include "tools/string.hpp"

#include <iostream>

#ifndef NDEBUG
    /// @todo trouver un moyen pour cacher ASSERTION2 ASSERTION1 et GET_MACRO
    #define ASSERTION( expression, message ) \
        too::assertion( expression,          \
                        __FILE__,            \
                        __LINE__,            \
                        __func__,            \
                        #expression,         \
                        message )
#else
    #define ASSERTION( expression, message )
#endif

namespace too
{
    void assertion( bool const & expression, std::string const & fileName,
                    int const & line, std::string const & functionName,
                    std::string const & expressionString,
                    std::string const & message = ""s );
} // namespace too