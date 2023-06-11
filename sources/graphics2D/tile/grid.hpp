#pragma once

#include <optional>  // for optional

#include "graphics2D/tile/cellpos.hpp"   // for CellPos
#include "graphics2D/tile/position.hpp"  // for Position
#include "graphics2D/tile/size.hpp"      // for Size
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F

namespace tile
{
    // TODO Inherit from Transformable2D
    // Abtract class for tilemap and tileset
    class Grid
    {
      public:
        Grid()          = default;
        virtual ~Grid() = default;

        virtual math::Vector2F get_position () const                       = 0;
        virtual void           set_position ( math::Vector2F const & pos ) = 0;
        virtual tile::Size     get_size () const                           = 0;

        // position of the bottom right corner
        math::Vector2F get_bound_pos () const;
        // Absolute position of the center of the tilemap in pixel
        math::Vector2F get_center_absolute () const;
        // Relative position of the center of the tilemap in pixel
        math::Vector2F   get_center_relative () const;
        math::RectangleF get_rect () const;

        std::optional< tile::CellPos > position (
            tile::Position const & pos ) const;
        std::optional< tile::CellPos > position ( unsigned int index ) const;

        std::optional< tile::CellPos > get_cell_pos (
            math::Vector2F const & absolutePos ) const;
        std::optional< math::Vector2F > get_cell_abs_pos (
            math::Vector2F const & absolutePos ) const;

        bool is_compatible ( tile::CellPos const & position ) const;
        bool contains ( tile::Position const & position ) const;
        bool contains_abs ( math::Vector2F const & absPos ) const;
    };

    // class ConcreteGrid : public Grid
    // {
    //     math::Vector2F m_position;
    //     tile::Size     m_size;

    // public:
    //   ConcreteGrid( math::Vector2F position, tile::Size size );

    // math::Vector2F get_position () const override;
    // void           set_position ( math::Vector2F pos ) override;
    // tile::Size     get_size () const override;
    // };
}  // namespace tile
