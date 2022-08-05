#include "image_map.hpp"

#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget

#include "main/render.hpp"  // for Render

ImageMap::ImageMap() : m_data()
{
    /// @todo load the the sprites path and the positions from the database from
    /// the database

    m_data = {
        /*
            {sf::Sprite { resources::get_texture( "happy_square.png"s ) },
             { 0u, 0u }  },
            { sf::Sprite { resources::get_texture( "logo.png"s ) },
             { 100u, 0u }},
            { sf::Sprite { resources::get_texture( "town_hall.png"s ) },
             { 0u, 200u }}, */
    };
}

std::vector< S_Image > const & ImageMap::get_data() const
{
    return m_data;
}

void ImageMap::render_before( Render & render ) const
{
    for ( S_Image const & image : m_data )
    {
        render.get_target().draw( image.sprite, render.get_state() );
    }
}