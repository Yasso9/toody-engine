#pragma once

#include <exception>
#include <filesystem>

#include "tools/string.hpp"

namespace exception
{
    class System : public std::exception
    {
        std::string const m_message;

      public:
        System( std::string const & message ) : m_message( message ) {}
        virtual ~System() = default;

        virtual const char * what() const noexcept override
        {
            // Static is used to not return the adress of a local variable
            static std::string errorInfo {};
            errorInfo = "'"s + m_message + "'"s;

            return errorInfo.c_str();
        }
    };

    class FileIssue : public std::exception
    {
        std::filesystem::path const m_filePath;
        std::string const m_message;

      public:
        FileIssue( std::filesystem::path const & filePath,
                   std::string const & message = "" )
          : m_filePath( filePath ), m_message( message )
        {}
        virtual ~FileIssue() = default;

        virtual const char * what() const noexcept override
        {
            static std::string errorInfo {};
            errorInfo = "Issue with file : '"s + m_filePath.string() + "'"s;
            if ( ! m_message.empty() )
            {
                errorInfo += "Detail : '"s + m_message + "'"s;
            }

            return errorInfo.c_str();
        }
    };

    class FileLoadingIssue : public FileIssue
    {
        std::string const m_fileType;

      public:
        FileLoadingIssue( std::filesystem::path const & filePath,
                          std::string const & fileType,
                          std::string const & message = "" )
          : FileIssue( filePath, message ), m_fileType( fileType )
        {}
        virtual ~FileLoadingIssue() = default;

        virtual const char * what() const noexcept override
        {
            // Static is used to not return the adress of a local variable
            static std::string errorInfo {};
            errorInfo =
                FileIssue::what() + "Cannot load '"s + m_fileType + "'"s;

            return errorInfo.c_str();
        }
    };

    class EnumUnexcpected : public std::exception
    {
      public:
        EnumUnexcpected()          = default;
        virtual ~EnumUnexcpected() = default;

        virtual const char * what() const noexcept override
        {
            static std::string errorInfo {};
            errorInfo = "Unexpected Enum value"s;

            return errorInfo.c_str();
        }
    };

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
} // namespace exception
