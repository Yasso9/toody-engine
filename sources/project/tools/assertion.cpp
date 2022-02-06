#include "assertion.hpp"

namespace too
{
    void assertion( bool const & expression, std::string const & fileName,
                    int const & line, std::string const & functionName,
                    std::string const & expressionString,
                    std::string const & message )
    {
        if ( ! expression )
        {
            std::cerr << "Assertion failed : " << message << "\n"
                      << "Expected : (" << expressionString << ") == true\n"
                      << "Source : " << fileName << "\n"
                      << "Line : " << line << "\n"
                      << "Function Call : " << functionName << "\n";

            abort();
        }
    }
} // namespace too