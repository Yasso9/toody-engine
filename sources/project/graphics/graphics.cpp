#include "graphics.hpp"

#include "tools/resources.hpp"
#include "tools/sfml.hpp"

#include <cstdlib>
#include <iostream>

Shaders::Shaders()
{
    if ( ! sf::Shader::isAvailable() )
    {
        std::cerr << "Error : shaders not available" << std::endl;
    }

    sf::Shader shader {};

    // Load the vertex and the fragment shader
    if ( ! shader.loadFromFile( "shader.vert", "shader.frag" ) )
    {
        std::cerr << "Error : shaders cannot be loaded" << std::endl;
    }
}