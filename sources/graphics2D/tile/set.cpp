#include "set.hpp"

#include <cmath>   // for floor
#include <memory>  // for allocator

#include "graphics2D/constants.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZ...
#include "maths/geometry/point.tpp"  // for Point::Point<Type>, Point::is_i...
#include "maths/vector2.tpp"         // for operator+, Vector2::Vector2<Type>
#include "tools/assertion.hpp"       // for ASSERTION

namespace tile
{
    Set::Set( sf::Texture const & texture ) : m_texture { texture } {}

    sf::Texture const & Set::get_texture() const
    {
        return m_texture;
    }

    math::Vector2F Set::get_position() const
    {
        return { 0.f, 0.f };
    }

    void Set::set_position( math::Vector2F const & /* pos */ ) {}

    tile::Size Set::get_size() const
    {
        return tile::Size { m_texture.getSize(), tile::Type::Pixel };
    }
}  // namespace tile
