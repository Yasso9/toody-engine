#pragma once

#include "graphics2D/quad_vertex.hpp"
#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

class TileMap;
class TileSelector;

class Tile
{
    /// @brief The Tilemap where the Tile is drawed
    TileMap const & m_tilemap;
    /// @brief The Tilemap where the Tile is based/retrieved
    TileSelector const & m_tileSelector;

    int m_value;

    QuadVertex m_quadVertex;
    /// @brief position of the tile in the tileset
    math::Vector2U tilesetPosition;
    // math::Vector2U tilemapPosition;

  public:
    Tile( TileMap const & tilemap, TileSelector const & tileSelector );

    sf::VertexArray const & get_vertex_array() const;
    int get_value() const;

    void set_data( int const & tilesetTileValue,
                   math::Vector2U const & tilemapTilePosition );

    std::string get_debug_info() const;

    std::ostream & operator<<( std::ostream & stream ) const;
};

std::ostream & operator<<( std::ostream & stream, Tile const & tileData );