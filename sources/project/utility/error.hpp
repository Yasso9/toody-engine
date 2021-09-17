#include <string>
#include <stdexcept>

class FileError : public std::runtime_error
{
public:
    FileError( std::string const & message, std::string const & fileName )
        : std::runtime_error( message ), m_fileName( fileName )
    {

    }

    std::string get_file_name() const
    {
        return m_fileName;
    }

    virtual const char * what() const noexcept
    {
        std::string errorInfo {};
        errorInfo += std::runtime_error::what();
        errorInfo += "\nFile in question : \"";
        errorInfo += this->get_file_name();

        return errorInfo.c_str();
    }

private:
    std::string const m_fileName;
};