#include "image_map.hpp"

#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget

ImageMap::ImageMap() : m_data()
{
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

void ImageMap::render_before( RenderContext context ) const
{
    for ( S_Image const & image : m_data )
    {
        context.draw( image.sprite );
    }
}