#include "tile.hpp"

#include <sstream>

#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tilemap.hpp"
#include "tools/global_variable.hpp"

static math::RectangleF get_tile_rectangle_in_tilemap(
    math::Vector2F const & tilemapPosition,
    math::Vector2F const & tileCoordinate )
{
    math::RectangleF rectangle {};

    rectangle.position = tilemapPosition + ( tileCoordinate * TILE_PIXEL_SIZE );
    rectangle.size     = TILE_PIXEL_SIZE_VECTOR;

    return rectangle;
}

static math::RectangleF get_tile_rectangle_in_texture(
    int const & tileValue, std::size_t numberOfTile )
{
    std::div_t divisionValue { std::div( tileValue,
                                         static_cast< int >( numberOfTile ) ) };

    return math::RectangleF {
        math::Vector2I {divisionValue.rem, divisionValue.quot}
            * TILE_PIXEL_SIZE_I,
        TILE_PIXEL_SIZE_VECTOR
    };
}

Tile::Tile( TileMap const & tilemap, TileSelector const & tileSelector )
  : m_tilemap( tilemap ),
    m_tileSelector( tileSelector ),
    m_value( 0 ),
    m_quad( sf::Quads, 4 )
{
    this->set_data( 0, math::Vector2U { 0u, 0u } );
}

sf::VertexArray const & Tile::get_vertex_array() const
{
    return this->m_quad;
}

int Tile::get_value() const
{
    return this->m_value;
}

std::string Tile::get_debug_info() const
{
    std::ostringstream outputStream {};
    outputStream << "Tile : " << this->m_value << "\n"
                 << "Position ( " << this->m_quad[0].position << ", "
                 << this->m_quad[1].position << ", " << this->m_quad[2].position
                 << ", " << this->m_quad[3].position << " )"
                 << "\n"
                 << "TextCoord ( " << this->m_quad[0].texCoords << ", "
                 << this->m_quad[1].texCoords << ", "
                 << this->m_quad[2].texCoords << ", "
                 << this->m_quad[3].texCoords << " )";

    return outputStream.str();
}

void Tile::set_value( int const & tilesetTileValue )
{
    /// @todo what do we do if tilesetTileValue is negativ ?

    this->m_value = tilesetTileValue;

    this->update_texture_coordinate();
}

void Tile::set_position( math::Vector2F const & tileCoordinate )
{
    math::RectangleF const rectangle { get_tile_rectangle_in_tilemap(
        this->m_tilemap.getPosition(),
        tileCoordinate ) };

    this->m_quad[0].position = rectangle.position;
    this->m_quad[1].position =
        rectangle.position + math::Vector2F { rectangle.size.x, 0.f };
    this->m_quad[2].position = rectangle.position + rectangle.size;
    this->m_quad[3].position =
        rectangle.position + math::Vector2F { 0.f, rectangle.size.y };
}

void Tile::set_data( int const & tilesetTileValue,
                     math::Vector2U const & tilemapTilePosition )
{
    this->set_value( tilesetTileValue );

    this->set_position( tilemapTilePosition );
}

std::ostream & Tile::operator<<( std::ostream & stream ) const
{
    return stream << this->m_value;
}

void Tile::update_texture_coordinate()
{
    math::RectangleF textureTileRectangle { get_tile_rectangle_in_texture(
        this->m_value,
        this->m_tileSelector.get_tileset().get_size_in_tile().x ) };

    this->m_quad[0].texCoords = textureTileRectangle.position;
    this->m_quad[1].texCoords =
        textureTileRectangle.position
        + math::Vector2F { textureTileRectangle.size.x, 0.f };
    this->m_quad[2].texCoords =
        textureTileRectangle.position + textureTileRectangle.size;
    this->m_quad[3].texCoords =
        textureTileRectangle.position
        + math::Vector2F { 0.f, textureTileRectangle.size.y };
}

std::ostream & operator<<( std::ostream & stream, Tile const & tileData )
{
    tileData.operator<<( stream );

    return stream;
}