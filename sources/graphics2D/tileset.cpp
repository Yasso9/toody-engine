#include "tileset.hpp"

#include "tools/global_variable.hpp"

Tileset::Tileset( sf::Texture const & texture, math::Vector2D const & position )
  : m_texture( texture ), m_position( position )
{}

sf::Texture const & Tileset::get_texture()
{
    return this->m_texture;
}

math::Vector2D Tileset::get_pixel_size() const
{
    return this->m_texture.getSize();
}

math::Vector2D Tileset::get_tile_size() const
{
    sf::Vector2f maxNumberOfTile {};
    maxNumberOfTile.x =
        std::floor( this->get_pixel_size().x / TILE_PIXEL_SIZE );
    maxNumberOfTile.y =
        std::floor( this->get_pixel_size().y / TILE_PIXEL_SIZE );
    return maxNumberOfTile;
}

math::Vector2D Tileset::get_tile_position_in_pixel(
    math::Vector2D const & pointPosition, bool isRelativePositon ) const
{
    // Calculate the position of the selection rectangle
    return {
        this->get_tile_position_in_tile( pointPosition, isRelativePositon )
            * sf::Vector2f {TILE_PIXEL_SIZE, TILE_PIXEL_SIZE}
        + this->m_position
    };
}
math::Vector2D Tileset::get_tile_position_in_tile(
    math::Vector2D const & pointPosition, bool isRelativePositon ) const
{
    math::Vector2D relativPointPosition { pointPosition };
    if ( ! isRelativePositon )
    {
        relativPointPosition -= this->m_position;
    }

    math::Vector2D selectionTilePosition {};
    selectionTilePosition.x = std::floor( pointPosition.x / TILE_PIXEL_SIZE );
    selectionTilePosition.y = std::floor( pointPosition.y / TILE_PIXEL_SIZE );
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
    float tileSelected { pointPositionInTile.x
                         + pointPositionInTile.y * this->get_tile_size().x };

    // TYPO ASSERT that tileSelected is >0 and <(maxNumberOfTile.x * maxNumberOfTile.y)

    return static_cast< int >( tileSelected );
}