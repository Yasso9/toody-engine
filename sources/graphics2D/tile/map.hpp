#pragma once

#include <iostream>  // for ostream
#include <vector>    // for vector

#include "component/transformable2D.hpp"  // for TransformableComponent
#include "graphics2D/tile/cursor.hpp"
#include "graphics2D/tile/selector.hpp"  // for TileSelector
#include "graphics2D/tile/table.hpp"     // for Tile
#include "graphics2D/tile/tile.hpp"      // for Tile
#include "maths/vector2.hpp"             // for Vector2F, Vector2U
#include "maths/vector2.tpp"             // for operator/, Vector2::oper...
#include "tools/databases.hpp"

class Render;
class Set;
class View;

namespace tile
{
    /// @todo have a Map and Set that are dependant. Maybe create an abstract
    /// class.
    class Map : public Transformable2D
    {
        tile::Selector m_tileSelector;
        tile::Table    m_table;

        tile::Cursor m_cursor;
        /// @brief view of the component that call the tilemap
        View &       m_view;

        std::filesystem::path m_saveFile;

      public:
        explicit Map( View & view );
        virtual ~Map() = default;

      private:
        void update ( float deltaTime ) override;

      public:
        /// @brief Reference to the tileset that the tilemap use
        tile::Set const & get_tileset () const;
        /// @brief Size of the tilemap
        tile::Size        get_size () const;
        /// @brief Absolute position of the center of the tilemap in pixel
        math::Vector2F    get_center_absolute () const;
        /// @brief Relative position of the center of the tilemap in pixel
        math::Vector2F    get_center_relative () const;

        /// @brief Get the tile position of a point in the tilemap
        /// @return optionnal tile position if the point is in the tilemap
        std::optional< tile::Position > get_tile_position (
            math::PointF point ) const;

        bool contain ( math::PointF point ) const;
        bool is_comptatible ( tile::Position position ) const;

        /// @brief save the tilemap table into the database
        void save () const;

      private:
        void change_tile ( tile::Position position,
                           tile::Position value );
    };
}  // namespace tile