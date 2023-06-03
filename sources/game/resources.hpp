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

namespace resources
{
    sf::Texture const & get_texture ( fspath const & file );
    sf::Font const &    get_font ( fspath const & file );
    sf::Shader &        get_shader ( std::string const & vertexShaderFile,
                                     std::string const & fragmentShaderFile );

    fspath const FOLDER { "resources" };

    namespace fonts
    {
        fspath const FOLDER { resources::FOLDER / "fonts" };

        fspath const ARIAL { fonts::FOLDER / "arial.ttf" };
        fspath const INTER_REGULAR { fonts::FOLDER / "Inter-Regular.ttf" };
        fspath const SATOSHI_VARIABLE { fonts::FOLDER / "SatoshiVariable.ttf" };
        fspath const SWITZER_VARIABLE { fonts::FOLDER / "SwitzerVariable.ttf" };
        fspath const TUFFY { fonts::FOLDER / "tuffy.ttf" };
    }  // namespace fonts
};     // namespace resources
