#pragma once

#include <filesystem>
#include <string>  // for string

namespace sf
{
    class Font;
    class Shader;
    class Texture;
}  // namespace sf

namespace resources
{
    sf::Texture const & get_texture ( std::filesystem::path const & file );
    sf::Font const &    get_font ( std::filesystem::path const & file );
    /// @todo because of the deletion of the copy operator, we cannot return
    /// const for a modified shader, so replace shf::Shader by a custom created
    /// shader
    sf::Shader &        get_shader ( std::string const & vertexShaderFile,
                                     std::string const & fragmentShaderFile );
};  // namespace resources