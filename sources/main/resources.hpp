#pragma once

#include <map>

#include "graphics2D/sfml.hpp"
#include "tools/singleton.hpp"

class Resources final : public Singleton< Resources >
{
    friend Resources & Singleton< Resources >::get_instance();

  public:
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

    virtual ~Resources() = default;

    sf::Texture const & get_texture(
        Resources::E_TextureKey const & textureKey );
    sf::Font const & get_font( Resources::E_FontKey const & fontKey );

  private:
    Resources();

    std::map< Resources::E_TextureKey, sf::Texture > m_textures;
    std::map< Resources::E_FontKey, sf::Font > m_fonts;
};