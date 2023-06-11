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
    // TODO change name to Tilemap
    class Map : public Transformable2D,
                public Grid
    {
        std::shared_ptr< tile::Set const > m_tileset;
        tile::Table                        m_table;
        tile::MouseCursor                  m_cursor;

      public:
        explicit Map( std::shared_ptr< tile::Set const > tileset );
        virtual ~Map() = default;

      private:
        void update ( UpdateContext & context ) override;

      public:
        math::Vector2F get_position () const override;
        void           set_position ( math::Vector2F const & pos ) override;
        tile::Size     get_size () const override;

        std::shared_ptr< tile::Set const > get_tileset () const;

        void load ( std::filesystem::path const & path );
        void save () const;

      private:
        void change_tile ( tile::CellPos position, tile::CellPos value );

        void update_size ( UpdateContext & context );
        void update_informations ( UpdateContext & context );
    };
}  // namespace tile
