#pragma once

#include <string>  // for string

#include <SFML/Graphics/Texture.hpp>  // for Texture
#include <assimp/material.h>          // for aiTextureType

// struct Texture
// {
//     enum class E_Type
//     {
//         Diffuse = 0,
//         Specular,
//         Normal,
//         Height
//     };

// Texture::E_Type m_type;
// std::string m_path;
// unsigned int id;

// static aiTextureType to_assimp_type( Texture::E_Type const & type );

// Texture::E_Type get_type() const;
// std::string get_path() const;
// std::string get_type_name() const;}
;

class Texture final : public sf::Texture
{
  public:
    enum class E_Type
    {
        Diffuse = 0,
        Specular,
        Normal,
        Height
    };

    Texture( std::string const & filePath, Texture::E_Type const & type );

    Texture::E_Type get_type () const;
    std::string     get_path () const;
    std::string     get_type_name () const;

    static aiTextureType to_assimp_type ( Texture::E_Type const & type );

  private:
    Texture::E_Type m_type;
    std::string     m_path;
};

namespace GLTexture
{
    unsigned int load ( std::string const & filePath );
}  // namespace GLTexture