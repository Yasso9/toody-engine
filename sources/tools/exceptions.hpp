#pragma once

#include <exception>
#include "tools/string.hpp"

namespace Exception
{
    class Database final : public std::exception
    {
      public:
        Database( std::string const & databasePath,
                  std::string const & errorMessage = ""s );
        virtual ~Database() = default;

        virtual const char * what() const noexcept override;

      private:
        std::string const m_databasePath;
        std::string const m_errorMessage;
    };

    class FileNotFound final : public std::exception
    {
      public:
        FileNotFound( std::string const & fileName );
        virtual ~FileNotFound() = default;

        virtual const char * what() const noexcept override;

      private:
        std::string const m_fileName;
    };

    /// @brief Excpetion to launch when we want to exit of the application
    class QuitApplication final : public std::exception
    {
      public:
        QuitApplication()          = default;
        virtual ~QuitApplication() = default;

        virtual const char * what() const noexcept override;
    };
} // namespace Exception
