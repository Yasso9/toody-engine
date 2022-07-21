#pragma once

#include "graphics2D/sfml.hpp"

namespace resources
{
    sf::Texture const & get_texture( std::string const & file );
    sf::Font const & get_font( std::string const & file );
    /// @todo because of the deletion of the copy operator, we cannot return const for a modified shader, so replace shf::Shader by a custom created shader
    sf::Shader & get_shader( std::string const & vertexShaderFile,
                             std::string const & fragmentShaderFile );
}; // namespace resources