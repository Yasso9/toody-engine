#include "sprite.hpp"

#include "main/render.hpp"
#include "tools/global_variable.hpp"

Sprite::Sprite( sf::Texture const & texture ) : sf::Sprite { texture }
{
    this->select_animation( 0 );
}

void Sprite::render( Render & render ) const
{
    /// @todo rewrite all render draw like this
    render.draw( *this );
}

math::PolygonF Sprite::get_polygon() const
{
    math::RectangleF spriteRectangle { 0.f, 0.f, 0.f, 0.f };

    spriteRectangle.position = math::PointF { this->getPosition() };
    spriteRectangle.size.x =
        static_cast< float >( this->getTextureRect().width );
    spriteRectangle.size.y =
        static_cast< float >( this->getTextureRect().height );

    return spriteRectangle;
}

void Sprite::select_animation( unsigned int index )
{
    sf::IntRect textureRect {};
    textureRect.left   = static_cast< int >( index ) * TILE_PIXEL_SIZE_I;
    textureRect.top    = 0;
    textureRect.width  = TILE_PIXEL_SIZE_I;
    textureRect.height = TILE_PIXEL_SIZE_I;

    if ( math::Vector2I { textureRect.left, textureRect.top }
             + math::Vector2I { textureRect.width, textureRect.height }
         > math::Vector2I {
             this->getTextureRect().width, this->getTextureRect().height } )
    {
        std::cerr << "Cannot select sprite " << index << ". Texture too small"
                  << std::endl;
        return;
    }
    this->setTextureRect( textureRect );
}
