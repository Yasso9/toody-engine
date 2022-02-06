#pragma once

#include <map>
#include <string>
#include <utility>

#include "tools/sfml.hpp"

using T_KeyboardInputMap =
    std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>>;

using T_MouseInputMap =
    std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>;

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

using T_TexturesMap = std::map<E_TextureKey, sf::Texture>;
using T_FontsMap = std::map<E_FontKey, sf::Font>;