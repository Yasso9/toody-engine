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

class Tile
{
    /// @brief The Tilemap where the Tile is drawed
    TileMap const & m_tilemap;
    /// @brief The Tilemap where the Tile is based/retrieved
    Tileset const & m_tileset;
    /// @brief Tile Data for drawing
    QuadVertex      m_quadVertex;

  public:
    Tile( TileMap const & tilemap, Tileset const & tileset );

    Tile( Tile const & tile ) noexcept;
    Tile( Tile && tile ) noexcept;
    Tile & operator= ( Tile const & tile ) noexcept;
    Tile & operator= ( Tile && tile ) noexcept;

    sf::VertexArray const & get_vertex_array () const;
    tile::Position          get_tilemap_position () const;
    tile::Position          get_tileset_position () const;

    void set_tileset_position ( tile::Position const & position );
    void set_tilemap_position ( tile::Position const & position );

    /// @brief Get all the current data of the tile
    /// @return String containing the current data of the tile
    std::string    get_debug () const;
    /// @brief Stream to output the value of the Tile in the Tileset
    /// @param stream Stream where the data is written
    /// @return
    std::ostream & operator<< ( std::ostream & stream ) const;
};