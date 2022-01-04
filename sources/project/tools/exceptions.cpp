#include "exceptions.hpp"

#include <project/tools/string.hpp>

DatabaseException::DatabaseException( std::string const & databasePath,
                                      std::string const & errorMessage )
  : m_databasePath( databasePath ), m_errorMessage( errorMessage )
{
}

const char * DatabaseException::what() const noexcept
{
    // Static is used to not return the adress of a local variable
    static std::string const errorInfo { "\nIssue with file : "s
                                         + this->m_databasePath + "\nError : "s
                                         + this->m_errorMessage + "\n\n"s };

    return errorInfo.c_str();
}

FileNotFoundException::FileNotFoundException( std::string const & fileName )
  : m_fileName( fileName )
{
}

const char * FileNotFoundException::what() const noexcept
{
    // Static is used to not return the adress of a local variable
    static std::string const errorInfo { "\nFile not found : "s
                                         + this->m_fileName + "\n\n"s };

    return errorInfo.c_str();
}