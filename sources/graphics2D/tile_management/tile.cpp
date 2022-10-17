#include "tile.hpp"

#include <sstream>  // for operator<<, basic_ostream

#include <SFML/Graphics/Vertex.hpp>       // for Vertex
#include <SFML/Graphics/VertexArray.hpp>  // for VertexArray

#include "graphics2D/sfml.hpp"                           // for operator<<
#include "graphics2D/tile_management/tile_position.hpp"  // for tile::Position, tile::Position:...
#include "graphics2D/tile_management/tile_selector.hpp"  // for TileSelector
#include "graphics2D/tile_management/tilemap.hpp"        // for TileMap
#include "graphics2D/tile_management/tileset.hpp"        // for Tileset
#include "maths/geometry/point.hpp"                      // for Point
#include "maths/geometry/point.tpp"      // for Point::Point<Type>
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2U, Vector2F, Vector2
#include "maths/vector2.tpp"             // for Vector2::operator=, operator+
#include "tools/global_variable.hpp"     // for TILE_PIXEL_SIZE_VECTOR

Tile::Tile( TileMap const & tilemap, Tileset const & tileset )
  : m_tilemap { tilemap }, m_tileset { tileset }, m_quadVertex {}
{
    this->set_tileset_position( tile::Position {
        {0u, 0u},
        m_tileset.get_size(), tile::Position::Tile
    } );

    this->set_tilemap_position( tile::Position {
        {0u, 0u},
        tilemap.get_size(), tile::Position::Tile
    } );
}

Tile::Tile( Tile const & tile ) noexcept
  : m_tilemap { tile.m_tilemap },
    m_tileset { tile.m_tileset },
    m_quadVertex { tile.m_quadVertex }
{}

Tile::Tile( Tile && tile ) noexcept
  : m_tilemap { tile.m_tilemap },
    m_tileset { tile.m_tileset },
    m_quadVertex { std::exchange( tile.m_quadVertex, {} ) }
{}

Tile & Tile::operator= ( Tile const & tile ) noexcept
{
    return *this = Tile { tile };
}

Tile & Tile::operator= ( Tile && tile ) noexcept
{
    std::swap( m_quadVertex, tile.m_quadVertex );
    return *this;
}

sf::VertexArray const & Tile::get_vertex_array() const
{
    return m_quadVertex.array;
}

tile::Position Tile::get_tilemap_position() const
{
    return tile::Position {
        ( m_quadVertex.get_position()
          - math::Vector2F { m_tilemap.getPosition() } )
            .to_u_int(),
        m_tileset.get_size().tile().x, tile::Position::Pixel };
}

tile::Position Tile::get_tileset_position() const
{
    return tile::Position {
        m_quadVertex.get_texture_position().to_u_int(),
        m_tileset.get_size().tile().x, tile::Position::Pixel };
}

std::string Tile::get_debug_info() const
{
    std::ostringstream outputStream {};
    outputStream << "Tile : " << this->get_tileset_position().value() << "\n"
                 << "Position ( " << m_quadVertex.array[0].position << ", "
                 << m_quadVertex.array[1].position << ", "
                 << m_quadVertex.array[2].position << ", "
                 << m_quadVertex.array[3].position << " )"
                 << "\n"
                 << "TextCoord ( " << m_quadVertex.array[0].texCoords << ", "
                 << m_quadVertex.array[1].texCoords << ", "
                 << m_quadVertex.array[2].texCoords << ", "
                 << m_quadVertex.array[3].texCoords << " )";

    return outputStream.str();
}

void Tile::set_tilemap_position( tile::Position const & position )
{
    math::RectangleF positionRectangle {};

    positionRectangle.position = math::Vector2F { m_tilemap.getPosition() }
                                 + position.pixel().to_float();
    positionRectangle.size = TILE_PIXEL_SIZE_VECTOR;

    m_quadVertex.set_position( positionRectangle );
}

void Tile::set_tileset_position( tile::Position const & position )
{
    math::RectangleF textureTileRectangle {};

    textureTileRectangle.position = position.pixel().to_float();
    textureTileRectangle.size     = TILE_PIXEL_SIZE_VECTOR;

    m_quadVertex.set_texture_coord( textureTileRectangle );
}

std::ostream & Tile::operator<< ( std::ostream & stream ) const
{
    return stream << this->get_tileset_position().value();
}