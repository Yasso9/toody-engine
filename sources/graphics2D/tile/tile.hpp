#pragma once

#include <string>  // for string

#include "application/components/component.hpp"
#include "graphics2D/surface/quad_vertex.hpp"  // for QuadVertex

namespace tile
{
    class CellPos;
    class Map;
    class Set;
}  // namespace tile

namespace tile
{
    class Tile : public Component
    {
        QuadVertex m_quad;

      public:
        Tile();
        Tile( tile::CellPos position, tile::CellPos value );
        Tile( math::Vector2F pixelPosition, math::Vector2F pixelValue );

      private:
        void render ( RenderContext & context ) const override;

      public:
        math::Vector2F get_value () const;
        void           set_value ( math::Vector2F position );
        void           set_value ( tile::CellPos position );

        math::Vector2F get_position () const;
        void           set_position ( math::Vector2F position );
        void           set_position ( tile::CellPos position );

        /// @brief Get all the current data of the tile
        /// @return String containing the current data of the tile
        std::string get_debug () const;

        friend std::ostream & operator<< ( std::ostream & stream,
                                           Tile const &   tile );
        friend std::istream & operator>> ( std::istream & stream, Tile & tile );
    };
}  // namespace tile
