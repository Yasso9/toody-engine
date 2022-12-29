#include "exceptions.hpp"

namespace exception
{
    Database::Database( std::string const & databasePath,
                        std::string const & errorMessage /* = ""s */ )
      : m_databasePath( databasePath ), m_errorMessage( errorMessage )
    {}

    char const * Database::what() const noexcept
    {
        // Static is used to not return the adress of a local variable
        static std::string const errorInfo {
            "\nIssue with file : "s + this->m_databasePath + "\nError : "s
            + this->m_errorMessage + "\n\n"s };

        return errorInfo.c_str();
    }

    FileNotFound::FileNotFound( std::string const & fileName )
      : m_fileName( fileName )
    {}

    char const * FileNotFound::what() const noexcept
    {
        // Static is used to not return the adress of a local variable
        static std::string const errorInfo { "\nFile not found : "s
                                             + this->m_fileName + "\n\n"s };

        return errorInfo.c_str();
    }

}  // namespace exception