#include "tileset.hpp"

#include <cmath>   // for floor
#include <memory>  // for allocator

#include "maths/geometry/point.tpp"   // for Point::Point<Type>, Point::is_i...
#include "maths/vector2.tpp"          // for operator+, Vector2::Vector2<Type>
#include "tools/assertion.hpp"        // for ASSERTION
#include "tools/global_variable.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZ...

Tileset::Tileset( sf::Texture const & texture, math::Vector2F position )
  : m_texture { texture }, m_position { position }
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
    return this->get_position() + this->get_size().pixel();
}

tile::Size Tileset::get_size() const
{
    return tile::Size { m_texture.getSize(), tile::Size::Pixel };
}

unsigned int Tileset::get_number_of_tile() const
{
    math::Vector2U tileSize { this->get_size().tile() };
    return tileSize.x * tileSize.y;
}

unsigned int Tileset::get_number_of_columns() const
{
    return static_cast< unsigned int >(
               std::floor( this->m_texture.getSize().x ) )
           / TILE_PIXEL_SIZE_U;
}

tile::Position Tileset::get_tile_position( math::PointF point ) const
{
    return tile::Position {
        point.to_vector().to_u_int(), this->get_number_of_columns(),
        tile::Position::Pixel };
}

void Tileset::set_position( math::Vector2F const & position )
{
    this->m_position = position;
}

bool Tileset::contain( math::PointF const & point ) const
{
    return point.is_inside(
        this->get_position(), this->get_size().pixel().to_float() );
}
