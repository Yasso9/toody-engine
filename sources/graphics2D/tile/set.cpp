#include "set.hpp"

#include <cmath>   // for floor
#include <memory>  // for allocator

#include "graphics2D/constants.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZ...
#include "maths/geometry/point.tpp"  // for Point::Point<Type>, Point::is_i...
#include "maths/vector2.tpp"         // for operator+, Vector2::Vector2<Type>
#include "tools/assertion.hpp"       // for ASSERTION

namespace tile
{
    Set::Set( sf::Texture const & texture )
      : m_texture { texture }, m_position { 0.f, 0.f }
    {}

    sf::Texture const & Set::get_texture() const
    {
        return m_texture;
    }

    math::Vector2F Set::get_position() const
    {
        return m_position;
    }

    void Set::set_position( math::Vector2F pos )
    {
        m_position = pos;
    }

    math::Vector2F Set::get_bound_pos() const
    {
        return this->get_position() + this->get_size().pixel().to_float();
    }

    tile::Size Set::get_size() const
    {
        return tile::Size { m_texture.getSize(), tile::Size::Pixel };
    }

    unsigned int Set::get_number_of_tile() const
    {
        math::Vector2U sizeInTile { this->get_size().tile() };
        return sizeInTile.x * sizeInTile.y;
    }

    tile::Position Set::get_position( math::PointF         point,
                                      tile::Position::Type type ) const
    {
        return tile::Position { point.to_u_int(), this->get_size(), type };
    }

    tile::Position Set::get_position( unsigned int value ) const
    {
        return tile::Position { value, this->get_size() };
    }

    bool Set::contain( math::PointF point ) const
    {
        return point.is_inside( this->get_position(),
                                this->get_size().pixel().to_float() );
    }
}  // namespace tile
