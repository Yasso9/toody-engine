#include "tools.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace tools
{
    namespace get_path
    {
        std::string resources( std::string const & fileName /* = ""s */ )
        {
            return "./data/resources/"s + fileName;
        }
        std::string shaders( std::string const & fileName /* = ""s */ )
        {
            return tools::get_path::resources() + "shaders/"s + fileName;
        }
        std::string databases( std::string const & fileName /* = ""s */ )
        {
            return tools::get_path::resources() + "databases/"s + fileName;
        }
    } // namespace get_path

    bool is_in_part( sf::Vector2f const value, sf::Vector2f const position,
                     sf::Vector2f const size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }

    bool is_integer( float const & number )
    {
        return std::trunc( number ) != number;
    }

    std::string read_file( std::string const & fileName )
    {
        std::ifstream file { fileName, std::ios::in };
        if ( ! file )
        {
            std::cerr << "File not found" << fileName << std::endl;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void * cast_int_to_void_pointer( int const & integer )
    {
        return reinterpret_cast< void * >( static_cast< intptr_t >( integer ) );
    }
    void * cast_unsigned_int_to_void_pointer( int const & unsignedInteger )
    {
        return reinterpret_cast< void * >(
            static_cast< uintptr_t >( unsignedInteger ) );
    }

} // namespace tools