#pragma once

#include "graphics2D/quad_vertex.hpp"
#include "graphics2D/sfml.hpp"
#include "graphics2D/tile_position.hpp"
#include "maths/maths.hpp"

class TileMap;
class TileSelector;

class Tile
{
    /// @brief The Tilemap where the Tile is drawed
    TileMap const & m_tilemap;
    /// @brief The Tilemap where the Tile is based/retrieved
    TileSelector const & m_tileSelector;

    QuadVertex m_quadVertex;

  public:
    Tile( TileMap const & tilemap, TileSelector const & tileSelector );

    sf::VertexArray const & get_vertex_array() const;
    TilePosition get_position_in_tilemap() const;
    TilePosition get_position_in_tileset() const;

    void set_positions( TilePosition const & tileset,
                        TilePosition const & tilemap );
    void set_position_in_tileset( TilePosition const & tileset );

    std::string get_debug_info() const;

    std::ostream & operator<<( std::ostream & stream ) const;
};

/// @todo faire qqch pour pas avoir ça a chaque fois
std::ostream & operator<<( std::ostream & stream, Tile const & tileData );