#include "tile.hpp"

#include <sstream>

#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tilemap.hpp"
#include "tools/assertion.hpp"
#include "tools/global_variable.hpp"

Tile::Tile( TileMap const & tilemap, TileSelector const & tileSelector )
  : m_tilemap( tilemap ),
    m_tileSelector( tileSelector ),
    m_value( 0 ),
    m_quadVertex(),
    tilesetPosition( 0u, 0u )
{
    this->set_data( 0, math::Vector2U { 0u, 0u } );
}

sf::VertexArray const & Tile::get_vertex_array() const
{
    return this->m_quadVertex.array;
}

int Tile::get_value() const
{
    return this->m_value;
}

std::string Tile::get_debug_info() const
{
    std::ostringstream outputStream {};
    outputStream << "Tile : " << this->m_value << "\n"
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

void Tile::set_data( int const & tilesetTileValue,
                     math::Vector2U const & tilemapTilePosition )
{
    // @todo what do we do if tilesetTileValue is negativ ?

    this->m_value = tilesetTileValue;

    std::div_t divisionValue { std::div(
        this->m_value,
        static_cast< int >(
            this->m_tileSelector.get_tileset().get_size_in_tile().x ) ) };
    math::RectangleF textureTileRectangle {
        math::Vector2I {divisionValue.rem, divisionValue.quot}
         .to_float()
            * TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE_VECTOR
    };

    this->m_quadVertex.set_texture_coord( textureTileRectangle );

    math::RectangleF positionRectangle {};
    positionRectangle.position =
        math::Vector2F { this->m_tilemap.getPosition() }
        + ( tilemapTilePosition * TILE_PIXEL_SIZE );
    positionRectangle.size = TILE_PIXEL_SIZE_VECTOR;

    this->m_quadVertex.set_position( positionRectangle );
}

std::ostream & Tile::operator<<( std::ostream & stream ) const
{
    return stream << this->m_value;
}

std::ostream & operator<<( std::ostream & stream, Tile const & tileData )
{
    tileData.operator<<( stream );

    return stream;
}