#pragma once

/// @todo rename this file to image_map.hpp

#include <filesystem>
#include <vector>

#include "graphics2D/sfml.hpp"
#include "main/resources.hpp"
#include "maths/maths.hpp"
#include "tools/path.hpp"

struct Image
{
    sf::Sprite sprite {};
    math::PointU position { 0u, 0u };

    Image( sf::Sprite aSprite, math::PointU aPosition )
      : sprite( aSprite ), position( aPosition )
    {}
};

class ImageMap : public sf::Drawable
{
    std::vector< Image > m_data;

  public:
    ImageMap() : m_data()
    {
        /// @todo load the the sprites path and the positions from the database from the database

        m_data = {
            {sf::Sprite { Resources::get_instance().get_texture(
                  "happy_square.png"s ) },
             { 0u, 0u }  },
            { sf::Sprite {
                  Resources::get_instance().get_texture( "logo.png"s ) },
             { 100u, 0u }},
            { sf::Sprite {
                  Resources::get_instance().get_texture( "townhall.png"s ) },
             { 0u, 200u }},
        };
    }
    virtual ~ImageMap() = default;

    std::vector< Image > const & get_data() const { return m_data; }

  private:
    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override
    {
        for ( auto const & image : m_data )
        {
            target.draw( image.sprite, states );
        }
    }
};