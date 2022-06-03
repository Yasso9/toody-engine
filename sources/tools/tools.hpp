#pragma once

#include <vector>

#include "graphics2D/sfml.hpp"
#include "tools/string.hpp"

namespace tools
{
    // TYPO Use std::filesystem
    /// @brief Get path of folders relatives to root
    namespace get_path
    {
        std::string data( std::string const & fileName = ""s );
        std::string resources( std::string const & fileName = ""s );
        std::string shaders( std::string const & fileName = ""s );
        std::string databases( std::string const & fileName = ""s );
    } // namespace get_path

    bool is_integer( float const & number );

    std::string read_file( std::string const & fileName );

    void * cast_int_to_void_pointer( int const & integer );
    void * cast_unsigned_int_to_void_pointer( int const & unsignedInteger );

    template < typename Type >
    std::size_t size_of( std::vector< Type > const & array );

    template < typename Type >
    Type const * to_c_style_array( std::vector< Type > const & array );
} // namespace tools

/// @brief Cardinal Directions
enum class E_Direction
{
    Up = 0,
    Down,
    Right,
    Left,
    EnumLast,
};

#include "tools.tpp"