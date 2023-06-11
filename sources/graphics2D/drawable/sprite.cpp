#include "sprite.hpp"

#include "graphics2D/constants.hpp"

Sprite::Sprite( sf::Texture const & texture, math::Vector2F size )
  : sf::Sprite { texture }, m_size { size }
{
    this->select_animation( 0 );
}

void Sprite::render( RenderContext & context ) const
{
    context.draw( *this );
}

math::PointF Sprite::get_position() const
{
    return math::PointF { this->getPosition() };
}

math::Vector2F Sprite::get_size() const
{
    return math::Vector2F {
        static_cast< float >( this->getTextureRect().width ),
        static_cast< float >( this->getTextureRect().height ) };
}

math::PolygonF Sprite::get_polygon() const
{
    math::RectangleF spriteRectangle {};

    spriteRectangle.pos = this->get_position();
    spriteRectangle.size     = this->get_size();

    return spriteRectangle;
}

void Sprite::select_animation( unsigned int index )
{
    sf::IntRect textureRect {};
    textureRect.left = static_cast< int >( index )
                       * static_cast< int >( m_size.x );
    textureRect.top    = 0;
    textureRect.width  = TILE_PIXEL_SIZE_I;
    textureRect.height = TILE_PIXEL_SIZE_I;

    if ( math::Vector2I { textureRect.left, textureRect.top }
             + math::Vector2I { textureRect.width, textureRect.height }
         > this->get_size().to_int() )
    {
        std::cerr << "Cannot select sprite " << index << ". Texture too small"
                  << std::endl;
        return;
    }
    this->setTextureRect( textureRect );
}
