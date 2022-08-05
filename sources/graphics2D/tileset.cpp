#include "tileset.hpp"

#include <cmath>   // for floor
#include <memory>  // for allocator

#include "maths/geometry/point.tpp"   // for Point::Point<Type>, Point::is_i...
#include "maths/vector2.tpp"          // for operator+, Vector2::Vector2<Type>
#include "tools/assertion.hpp"        // for ASSERTION
#include "tools/global_variable.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZ...

Tileset::Tileset( sf::Texture const & texture, math::Vector2F const & position )
  : m_texture( texture ), m_position( position )
{}

sf::Texture const & Tileset::get_texture() const
{
    return this->m_texture;
}

math::Vector2F Tileset::get_position() const
{
    return this->m_position;
}

math::Vector2F Tileset::get_end_position() const
{
    return this->get_position() + this->get_size_in_pixel();
}

math::Vector2U Tileset::get_size_in_pixel() const
{
    return math::Vector2U { this->m_texture.getSize() };
}

math::Vector2U Tileset::get_size_in_tile() const
{
    return math::floor( this->get_size_in_pixel() ) / TILE_PIXEL_SIZE_U;
}

std::size_t Tileset::get_number_of_tile() const
{
    return this->get_size_in_tile().x * this->get_size_in_tile().y;
}

math::Vector2F Tileset::get_tile_position_in_pixel(
    math::Vector2F const & pointPosition, bool isRelativePositon ) const
{
    // Calculate the position of the selection rectangle
    return {
        this->get_tile_position_in_tile( pointPosition, isRelativePositon )
            * TILE_PIXEL_SIZE_VECTOR
        + this->m_position };
}

math::Vector2F Tileset::get_tile_position_in_tile(
    math::Vector2F const & pointPosition, bool isRelativePositon ) const
{
    math::PointF relativPointPosition { pointPosition };
    if ( ! isRelativePositon )
    {
        relativPointPosition -= this->m_position;
    }

    ASSERTION(
        relativPointPosition.is_inside(
            { 0.f, 0.f }, this->get_size_in_pixel().to_float() ),
        "The pointer must be between the vector(0, 0) and "
        "vector(SIZE_X, SIZEY)" );

    math::Vector2F selectionTilePosition {};
    selectionTilePosition.x =
        std::floor( relativPointPosition.x / TILE_PIXEL_SIZE );
    selectionTilePosition.y =
        std::floor( relativPointPosition.y / TILE_PIXEL_SIZE );
    return selectionTilePosition;
}

int Tileset::get_tile_value_from_pixel_position(
    math::Vector2F const & pointPositionInPixel, bool isRelativePositon ) const
{
    math::Vector2F tileCoordinate { this->get_tile_position_in_tile(
        pointPositionInPixel, isRelativePositon ) };

    return this->get_tile_value_from_tile_position( tileCoordinate.to_u_int() );
}

int Tileset::get_tile_value_from_tile_position(
    math::PointU const & pointPositionInTile ) const
{
    ASSERTION(
        pointPositionInTile.is_inside( { 0u, 0u }, this->get_size_in_tile() ),
        "The pointer must be between the vector(0, 0) and "
        "vector(SIZE_X, SIZEY)" );

    unsigned int tileSelected {
        pointPositionInTile.x
        + ( pointPositionInTile.y
            * static_cast< unsigned int >( this->get_size_in_tile().x ) ) };

    return static_cast< int >( tileSelected );
}

void Tileset::set_position( math::Vector2F const & position )
{
    this->m_position = position;
}

bool Tileset::contain( math::PointF const & point ) const
{
    return point.is_inside(
        this->get_position(), this->get_size_in_pixel().to_float() );
}
