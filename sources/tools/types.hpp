#pragma once

#include <map>
#include <string>
#include <utility>

#include "tools/sfml.hpp"

enum class E_TextureKey
{
    Tileset = 0,
    Player,
    HomeWallpaper,
    EnumLast,
};

enum class E_FontKey
{
    Arial = 0,
    EnumLast,
};

using T_TexturesMap = std::map< E_TextureKey, sf::Texture >;
using T_FontsMap    = std::map< E_FontKey, sf::Font >;