#pragma once

#include <map>
#include <string>
#include <utility>

#include <project/tools/sfml.hpp>

using KeyboardInputMap =
    std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>>;

using MouseInputMap =
    std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>;

#include <project/tools/enumeration.hpp>

using TexturesMap = std::map<TextureKey, sf::Texture>;
using FontsMap = std::map<FontKey, sf::Font>;