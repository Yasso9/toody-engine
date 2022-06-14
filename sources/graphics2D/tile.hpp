#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

class TileMap;
class TileSelector;

class Tile
{
    TileMap const & m_tilemap;
    TileSelector const & m_tileSelector;

    int m_value;
    sf::VertexArray m_quad;

  public:
    Tile( TileMap const & tilemap, TileSelector const & tileSelector );

    explicit operator int() const;

    sf::VertexArray const & get_vertex_array() const;

    int get_value() const;
    std::string get_debug_info() const;

    void set_value( int const & tilesetTileValue );
    void set_data( int const & tilesetTileValue,
                   math::Vector2U const & tilemapTilePosition );

    std::ostream & operator<<( std::ostream & stream ) const;

  private:
    void set_position( math::Vector2F const & tilemapPosition,
                       math::Vector2F const & tileCoordinate );

    void set_texture_coordinate( int const & tileValue,
                                 std::size_t numberOfXAxisTile );
};

std::ostream & operator<<( std::ostream & stream, Tile const & tileData );