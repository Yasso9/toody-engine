#include "stream.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace stream
{
    bool ignore_next ( std::istream & stream, char character )
    {
        // Ignore spaces
        while ( stream.peek() == ' ' )
        {
            // Advence of one character
            stream.seekg( 1 + stream.tellg() );
        }

        if ( stream.eof() )
        {
            std::cerr << "End of file could not read character '" << character
                      << "'" << std::endl;
            return false;
        }

        if ( stream.peek() != character )
        {
            std::cerr << "We should have the character : '" << character
                      << "' and not '" << static_cast< char >( stream.peek() )
                      << "'" << std::endl;

            return false;
        }

        // Advence of one character
        stream.seekg( 1 + stream.tellg() );

        return true;
    }

    char peek_next ( std::istream & stream )
    {
        auto baseIndex = stream.tellg();

        // Ignore spaces
        while ( stream.peek() == ' ' )
        {
            // Advence of one character
            stream.seekg( 1 + stream.tellg() );
        }

        if ( stream.eof() )
        {
            return '\0';
        }

        auto character = stream.peek();

        stream.seekg( baseIndex );

        return static_cast< char >( character );
    }

    // That check if we are at the end of a stream or if there is only
    // whitespace remaining
    bool is_ended ( std::istream const & stream )
    {
        // Make a copy of the stream to avoid modifying the original
        std::istream streamCopy { stream.rdbuf() };

        char c;
        while ( streamCopy.get( c ) )
        {
            if ( ! std::isspace( c ) )
            {  // Found a non-whitespace character
                return false;
            }
        }
        // If we get here, we've either hit EOF or only found whitespace
        // characters
        return true;
    }

    std::string get_string ( std::ifstream const & stream )
    {
        // Copy the stream's content to a stringstream.
        std::stringstream ss;
        ss << stream.rdbuf();

        // Create a string using the range constructor, which takes two
        // iterators.
        // std::istreambuf_iterator<char>(stream) is an iterator that begins at
        // the current position of stream. std::istreambuf_iterator<char>() is a
        // special "end of stream" iterator. This constructor will read
        // characters from the stream until it reaches the end of the stream,
        // and initialize the string with the characters it reads.
        return std::string( std::istreambuf_iterator< char > { ss },
                            std::istreambuf_iterator< char > {} );
    }

}  // namespace stream

namespace fs
{
    std::string get_content ( std::filesystem::path const & file )
    {
        std::ifstream tilemapData { file };
        if ( tilemapData )
        {
            return stream::get_string( tilemapData );
        }

        tilemapData.close();
        return "";
    }
}  // namespace fs
