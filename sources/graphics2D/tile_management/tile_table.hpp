#pragma once

#include <sstream>
#include <vector>

#include "graphics2D/tile_management/tile.hpp"
#include "graphics2D/tile_management/tile_position.hpp"
#include "maths/vector2.hpp"

class Tiles
{
    /// @brief list of tiles that have the same tilemap positions on different
    /// depth
    std::vector< Tile > m_vector;

  public:
    Tiles( std::vector< Tile > const & tiles );
    virtual ~Tiles() = default;
};

namespace tile
{
    /// @todo use that class in tilemap
    class Table
    {
        std::vector< Tiles > m_matrix;
        tile::Size           m_size;

      public:
        Table( std::vector< Tiles > tiles, tile::Size size );
        Table( std::vector< std::vector< Tiles > > tiles );
        ~Table() = default;

        Tiles get_element ( tile::Position position ) const;
        Tiles get_element ( unsigned int column, unsigned int line ) const;
        std::vector< Tiles > get_line ( unsigned int i_line ) const;
        std::vector< Tiles > get_column ( unsigned int i_column ) const;
    };
}  // namespace tile