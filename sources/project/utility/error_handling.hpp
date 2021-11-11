#pragma once

#include <project/utility/utility.hpp>

class DatabaseException final : public std::exception
{
  public:
    DatabaseException( std::string const & databasePath,
                       std::string const & errorMessage = "" );

    virtual const char * what() const noexcept;

  private:
    std::string const m_databasePath;
    std::string const m_errorMessage;
};

class FileNotFoundException final : public std::exception
{
  public:
    FileNotFoundException( std::string const & fileName );

    virtual const char * what() const noexcept;

  private:
    std::string const m_fileName;
};