#include "tileset.hpp"

#include "tools/global_variable.hpp"

Tileset::Tileset( sf::Texture const & texture, math::Vector2D const & position )
  : m_texture( texture ), m_position( position )
{}

sf::Texture const & Tileset::get_texture() const
{
    return this->m_texture;
}

math::Vector2D Tileset::get_position() const
{
    return this->m_position;
}
math::Vector2D Tileset::get_end_position() const
{
    return this->get_position() + this->get_size_in_pixel();
}

math::Vector2D Tileset::get_size_in_pixel() const
{
    return this->m_texture.getSize();
}

math::Vector2D Tileset::get_size_in_tile() const
{
    sf::Vector2f maxNumberOfTile {};
    maxNumberOfTile.x =
        std::floor( this->get_size_in_pixel().x / TILE_PIXEL_SIZE );
    maxNumberOfTile.y =
        std::floor( this->get_size_in_pixel().y / TILE_PIXEL_SIZE );
    return maxNumberOfTile;
}

math::Vector2D Tileset::get_tile_position_in_pixel(
    math::Vector2D const & pointPosition, bool isRelativePositon ) const
{
    // Calculate the position of the selection rectangle
    return { this->get_tile_position_in_tile( pointPosition, isRelativePositon )
                 * TILE_PIXEL_SIZE_VECTOR
             + this->m_position };
}
math::Vector2D Tileset::get_tile_position_in_tile(
    math::Vector2D const & pointPosition, bool isRelativePositon ) const
{
    math::Vector2D relativPointPosition { pointPosition };
    if ( ! isRelativePositon )
    {
        relativPointPosition -= this->m_position;
    }

    ASSERTION( relativPointPosition.is_inside( { 0.f, 0.f },
                                               this->get_size_in_pixel() ),
               "The pointer must be between the vector(0, 0) and "
               "vector(SIZE_X, SIZEY)" );

    math::Vector2D selectionTilePosition {};
    selectionTilePosition.x =
        std::floor( relativPointPosition.x / TILE_PIXEL_SIZE );
    selectionTilePosition.y =
        std::floor( relativPointPosition.y / TILE_PIXEL_SIZE );
    return selectionTilePosition;
}

int Tileset::get_tile_value_from_pixel_position(
    math::Vector2D const & pointPositionInPixel, bool isRelativePositon ) const
{
    math::Vector2D tileCoordinate { this->get_tile_position_in_tile(
        pointPositionInPixel,
        isRelativePositon ) };

    return this->get_tile_value_from_tile_position( tileCoordinate );
}

int Tileset::get_tile_value_from_tile_position(
    math::Vector2D const & pointPositionInTile ) const
{
    ASSERTION(
        pointPositionInTile.is_inside( { 0.f, 0.f }, this->get_size_in_tile() ),
        "The pointer must be between the vector(0, 0) and "
        "vector(SIZE_X, SIZEY)" );

    float tileSelected { pointPositionInTile.x
                         + pointPositionInTile.y * this->get_size_in_tile().x };

    return static_cast< int >( tileSelected );
}

void Tileset::set_position( math::Vector2D const & position )
{
    this->m_position = position;
}

bool Tileset::contain( math::Vector2D const & point ) const
{
    return point.is_inside( this->get_position(), this->get_size_in_pixel() );
}
