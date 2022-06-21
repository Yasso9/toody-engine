#include "tile.hpp"

#include <sstream>

#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tilemap.hpp"
#include "tools/assertion.hpp"
#include "tools/global_variable.hpp"

Tile::Tile( TileMap const & tilemap, TileSelector const & tileSelector )
  : m_tilemap( tilemap ), m_tileSelector( tileSelector ), m_quadVertex()
{
    this->set_positions(
        TilePosition {
            0,
            this->m_tileSelector.get_tileset().get_size_in_tile().x },
        TilePosition { 0, this->m_tilemap.get_tile_size().x } );
}

sf::VertexArray const & Tile::get_vertex_array() const
{
    return this->m_quadVertex.array;
}

TilePosition Tile::get_position_in_tilemap() const
{
    return TilePosition {
        ( this->m_quadVertex.get_position()
          - math::Vector2F { this->m_tilemap.getPosition() } )
            .to_u_int(),
        this->m_tileSelector.get_tileset().get_size_in_tile().x,
        TilePosition::Pixel
    };
}
TilePosition Tile::get_position_in_tileset() const
{
    return TilePosition {
        this->m_quadVertex.get_texture_position().to_u_int(),
        this->m_tileSelector.get_tileset().get_size_in_tile().x,
        TilePosition::Pixel
    };
}

std::string Tile::get_debug_info() const
{
    std::ostringstream outputStream {};
    outputStream << "Tile : " << this->get_position_in_tileset().value() << "\n"
                 << "Position ( " << this->m_quadVertex.array[0].position
                 << ", " << this->m_quadVertex.array[1].position << ", "
                 << this->m_quadVertex.array[2].position << ", "
                 << this->m_quadVertex.array[3].position << " )"
                 << "\n"
                 << "TextCoord ( " << this->m_quadVertex.array[0].texCoords
                 << ", " << this->m_quadVertex.array[1].texCoords << ", "
                 << this->m_quadVertex.array[2].texCoords << ", "
                 << this->m_quadVertex.array[3].texCoords << " )";

    return outputStream.str();
}

void Tile::set_positions( TilePosition const & tileset,
                          TilePosition const & tilemap )
{
    this->set_position_in_tileset( tileset );

    math::RectangleF positionRectangle {};
    positionRectangle.position =
        math::Vector2F { this->m_tilemap.getPosition() } + tilemap.pixel();
    positionRectangle.size = TILE_PIXEL_SIZE_VECTOR;
    this->m_quadVertex.set_position( positionRectangle );
}

void Tile::set_position_in_tileset( TilePosition const & tileset )
{
    math::RectangleF textureTileRectangle {};
    textureTileRectangle.position = tileset.pixel().to_float();
    textureTileRectangle.size     = TILE_PIXEL_SIZE_VECTOR;
    this->m_quadVertex.set_texture_coord( textureTileRectangle );
}

std::ostream & Tile::operator<<( std::ostream & stream ) const
{
    return stream << this->get_position_in_tileset().value();
}

std::ostream & operator<<( std::ostream & stream, Tile const & tileData )
{
    tileData.operator<<( stream );

    return stream;
}