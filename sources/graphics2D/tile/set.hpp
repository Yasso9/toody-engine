#pragma once

#include <cstddef>  // for size_t

#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "graphics2D/tile/position.hpp"
#include "maths/geometry/point.hpp"  // for PointF, PointU
#include "maths/vector2.hpp"         // for Vector2F, Vector2U

namespace tile
{
    // Table of tiles
    class Set
    {
        sf::Texture const & m_texture;
        math::Vector2F      m_position;

      public:
        explicit Set( sf::Texture const & texture );
        ~Set() = default;

        sf::Texture const & get_texture () const;

        math::Vector2F get_position () const;
        void           set_position ( math::Vector2F pos );
        // position of the bottom right corner
        math::Vector2F get_bound_pos () const;
        tile::Size     get_size () const;
        unsigned int   get_number_of_tile () const;

        tile::Position get_position ( math::PointF point,
                                      tile::Type   type ) const;
        tile::Position get_position ( unsigned int value ) const;

        bool contain ( math::PointF point ) const;
    };
}  // namespace tile
