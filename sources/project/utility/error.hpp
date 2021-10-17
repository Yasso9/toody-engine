#pragma once

#include <project/utility/utility.hpp>

class FileError final : public std::exception
{
  public:
    FileError( std::string const & fileName );

    virtual const char * what() const noexcept;

  private:
    std::string const m_fileName;
};