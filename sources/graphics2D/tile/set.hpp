#pragma once

#include <cstddef>  // for size_t

#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "graphics2D/tile/grid.hpp"  // for Grid
#include "maths/geometry/point.hpp"  // for PointF, PointU
#include "maths/vector2.hpp"         // for Vector2F, Vector2U

namespace tile
{
    // Table of tiles
    class Set : public Grid
    {
        sf::Texture const & m_texture;
        math::Vector2F      m_position;

      public:
        explicit Set( sf::Texture const & texture );
        ~Set() = default;

        sf::Texture const & get_texture () const;

        math::Vector2F get_position () const override;
        void           set_position ( math::Vector2F pos ) override;
        tile::Size     get_size () const override;
    };
}  // namespace tile
