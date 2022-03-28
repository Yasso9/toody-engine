#pragma once

#include <exception>
#include "tools/string.hpp"

class DatabaseException final : public std::exception
{
  public:
    DatabaseException( std::string const & databasePath,
                       std::string const & errorMessage = ""s );
    virtual ~DatabaseException() = default;

    virtual const char * what() const noexcept override;

  private:
    std::string const m_databasePath;
    std::string const m_errorMessage;
};

class FileNotFoundException final : public std::exception
{
  public:
    FileNotFoundException( std::string const & fileName );
    virtual ~FileNotFoundException() = default;

    virtual const char * what() const noexcept override;

  private:
    std::string const m_fileName;
};