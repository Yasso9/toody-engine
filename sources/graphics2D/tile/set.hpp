#pragma once

#include <cstddef>  // for size_t

#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "graphics2D/tile/position.hpp"
#include "maths/geometry/point.hpp"  // for PointF, PointU
#include "maths/vector2.hpp"         // for Vector2F, Vector2U

namespace tile
{
    class Set
    {
        sf::Texture const & m_texture;
        /// @brief Absolute position of the tileset
        math::Vector2F      m_position;

      public:
        /// @param position Position in pixel
        explicit Set( sf::Texture const & texture,
                      math::Vector2F position = math::Vector2F { 0.f, 0.f } );
        virtual ~Set() = default;

        sf::Texture const & get_texture () const;
        math::Vector2F      get_position () const;
        math::Vector2F      get_end_position () const;

        tile::Size   get_size () const;
        unsigned int get_number_of_tile () const;
        unsigned int get_number_of_columns () const;

        tile::Position get_tile_position ( math::PointF point ) const;
        void           set_position ( math::Vector2F const & position );

        bool contain ( math::PointF const & point ) const;
    };
}  // namespace tile