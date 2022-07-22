#pragma once

#include <filesystem>
#include <vector>

#include "graphics2D/component.hpp"
#include "graphics2D/sfml.hpp"
#include "main/resources.hpp"
#include "maths/maths.hpp"
#include "tools/path.hpp"

struct S_Image
{
    sf::Sprite sprite {};
    math::PointU position { 0u, 0u };

    S_Image( sf::Sprite aSprite, math::PointU aPosition )
      : sprite( aSprite ), position( aPosition )
    {}
};

class ImageMap : public Component2D
{
    std::vector< S_Image > m_data;

  public:
    ImageMap() : m_data()
    {
        /// @todo load the the sprites path and the positions from the database from the database

        m_data = {
            {sf::Sprite { resources::get_texture( "happy_square.png"s ) },
             { 0u, 0u }  },
            { sf::Sprite { resources::get_texture( "logo.png"s ) },
             { 100u, 0u }},
            { sf::Sprite { resources::get_texture( "townhall.png"s ) },
             { 0u, 200u }},
        };
    }
    virtual ~ImageMap() = default;

    std::vector< S_Image > const & get_data() const { return m_data; }

  private:
    void render( Render & render ) const override
    {
        for ( S_Image const & image : m_data )
        {
            render.get_target().draw( image.sprite, render.get_state() );
        }
    }
};