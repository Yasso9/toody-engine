#pragma once

#include <sstream>
#include <vector>

#include "graphics2D/tile.hpp"
#include "maths/vector2.hpp"

class Tiles
{
  public:
    std::vector< Tile > vector;

    Tiles( std::vector< Tile > const & tiles );
    ~Tiles() = default;
};

class TileTable
{
    std::vector< Tiles > m_matrix;
    /// @todo only have the number of column, get the size by a method
    math::Vector2U       m_size;

  public:
    TileTable( std::vector< Tiles > tiles, math::Vector2U size );
    TileTable( std::vector< std::vector< Tiles > > tiles );
    ~TileTable() = default;

    Tiles get_element ( math::Vector2U position ) const;
    Tiles get_element ( unsigned int column, unsigned int line ) const;
    std::vector< Tiles > get_line ( unsigned int i_line ) const;
    std::vector< Tiles > get_column ( unsigned int i_column ) const;
};