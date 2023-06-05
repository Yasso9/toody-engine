#pragma once

#include <filesystem>
#include <string>  // for string

namespace sf
{
    class Font;
    class Shader;
    class Texture;
}  // namespace sf

using fspath = std::filesystem::path;

namespace resource
{
    fspath const FOLDER { "resources" };

    namespace app_data
    {
        fspath const FOLDER { "data" };

        fspath const SETTINGS { app_data::FOLDER / "settings.txt" };
        fspath const TILEMAP { app_data::FOLDER / "tilemap.txt" };

        fspath get_path ( std::string const & name );
    }  // namespace app_data

    namespace font
    {
        fspath const FOLDER { resource::FOLDER / "fonts" };

        fspath const ARIAL { font::FOLDER / "arial.ttf" };
        fspath const FIRA_MONO { font::FOLDER / "FiraMono-Regular.ttf" };
        fspath const INTER_REGULAR { font::FOLDER / "Inter-Regular.ttf" };
        fspath const SATOSHI_VARIABLE { font::FOLDER / "SatoshiVariable.ttf" };
        fspath const SWITZER_VARIABLE { font::FOLDER / "SwitzerVariable.ttf" };
        fspath const TUFFY { font::FOLDER / "tuffy.ttf" };

        fspath           get_path ( std::string const & name );
        sf::Font const & get ( std::string const & name );
    }  // namespace font

    namespace tileset
    {
        fspath const FOLDER { resource::FOLDER / "tilesets" };

        fspath              get_path ( std::string const & name );
        sf::Texture const & get ( std::string const & name );
    }  // namespace tileset

    namespace example
    {
        fspath const FOLDER { resource::FOLDER / "examples" };

        fspath const SPRITE { example::FOLDER / "gold_sprite.png" };
        fspath const HAPPY_SQUARE { example::FOLDER / "happy_square.png" };
        fspath const LOGO { example::FOLDER / "logo.png" };

        fspath get_path ( std::string const & name );
    }  // namespace example

    namespace object3D
    {
        fspath const FOLDER { resource::FOLDER / "object3D" };

        fspath get_path ( std::string const & name );
    }  // namespace object3D

    namespace shader
    {
        fspath const FOLDER { resource::FOLDER / "shaders" };

        fspath       get_path ( std::string const & name );
        sf::Shader & get ( std::string const & vertexName,
                           std::string const & fragmentName );
    }  // namespace shader

    namespace image
    {
        fspath const FOLDER { resource::FOLDER / "images" };

        fspath const MAIN_MENU { image::FOLDER / "main_menu.png" };

        fspath              get_path ( std::string const & name );
        sf::Texture const & get ( std::string const & name );
    }  // namespace image

    namespace character
    {
        fspath const FOLDER { resource::FOLDER / "character_sheets" };

        fspath const GOLD_SPRITE { character::FOLDER / "gold_sprite.png" };

        fspath              get_path ( std::string const & name );
        sf::Texture const & get ( std::string const & name );
    }  // namespace character
};     // namespace resource
