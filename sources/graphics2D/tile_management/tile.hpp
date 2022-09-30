#pragma once

#include <iosfwd>  // for ostream
#include <string>  // for string

#include "graphics2D/quad_vertex.hpp"  // for QuadVertex

namespace tile
{
    class Position;
}  // namespace tile
class TileMap;
class Tileset;

namespace sf
{
    class VertexArray;
}  // namespace sf

class Tile
{
    /// @brief The Tilemap where the Tile is drawed
    TileMap const & m_tilemap;
    /// @brief The Tilemap where the Tile is based/retrieved
    Tileset const & m_tileset;

    QuadVertex m_quadVertex;

  public:
    Tile( TileMap const & tilemap, Tileset const & tileSelector );

    Tile( Tile const & tile ) noexcept;
    Tile( Tile && tile ) noexcept;
    Tile & operator= ( Tile const & tile ) noexcept;
    Tile & operator= ( Tile && tile ) noexcept;

    sf::VertexArray const & get_vertex_array () const;
    tile::Position          get_position_in_tilemap () const;
    tile::Position          get_position_in_tileset () const;

    void set_positions (
        tile::Position const & tileset, tile::Position const & tilemap );
    void set_position_in_tileset ( tile::Position const & tileset );

    std::string    get_debug_info () const;
    std::ostream & operator<< ( std::ostream & stream ) const;
};