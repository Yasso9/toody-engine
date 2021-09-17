#pragma once

#include <project/utility/utility.hpp>

namespace RessourcesInit
{
    std::map< TextureKey, sf::Texture > init_textures();
    std::map< FontKey, sf::Font > init_fonts();
}