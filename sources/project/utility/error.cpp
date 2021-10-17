#include "error.hpp"

FileError::FileError( std::string const & fileName ) : m_fileName( fileName ) {}

const char * FileError::what() const noexcept
{
    std::string const errorInfo { "File not found : "s + this->m_fileName };

    return errorInfo.c_str();
}