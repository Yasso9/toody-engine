#pragma once

#include <filesystem>
#include <iostream>  // for ostream
#include <vector>    // for vector

#include "application/components/transformable2D.hpp"  // for TransformableComponent
#include "graphics2D/tile/cursor.hpp"
#include "graphics2D/tile/selector.hpp"  // for TileSelector
#include "graphics2D/tile/table.hpp"     // for Tile
#include "graphics2D/tile/tile.hpp"      // for Tile
#include "maths/vector2.hpp"             // for Vector2F, Vector2U
#include "maths/vector2.tpp"             // for operator/, Vector2::oper...

class Render;
class Set;
class View;

namespace tile
{
    class Map : public Transformable2D
    {
        tile::Selector m_tileSelector;
        tile::Table    m_table;

        tile::MouseCursor m_cursor;
        // view of the component that call the tilemap
        View & m_view;

      public:
        explicit Map( View & view );
        virtual ~Map() = default;

      private:
        void update ( UpdateContext & context ) override;

      public:
        tile::Set const & get_tileset () const;
        tile::Size        get_size () const;
        // Absolute position of the center of the tilemap in pixel
        math::Vector2F get_center_absolute () const;
        // Relative position of the center of the tilemap in pixel
        math::Vector2F get_center_relative () const;
        View const &   get_view () const;

        // optionnal tile position if the point is in the tilemap
        std::optional< tile::Position > get_position (
            math::PointF point ) const;

        bool contain ( math::PointF point ) const;
        bool is_comptatible ( tile::Position position ) const;

        void save () const;

      private:
        void change_tile ( tile::Position position, tile::Position value );

        void update_size ( UpdateContext & context );
        void update_informations ( UpdateContext & context );
    };
}  // namespace tile
