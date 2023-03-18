#include "tile.hpp"

#include <sstream>  // for operator<<, basic_ostream

#include <SFML/Graphics/Vertex.hpp>       // for Vertex
#include <SFML/Graphics/VertexArray.hpp>  // for VertexArray

#include "graphics2D/constants.hpp"  // for TILE_PIXEL_SIZE_VECTOR
#include "graphics2D/sfml.hpp"       // for operator<<
#include "graphics2D/tile/map.hpp"
#include "graphics2D/tile/selector.hpp"  // for TileSelector
#include "maths/geometry/point.hpp"      // for Point
#include "maths/geometry/point.tpp"      // for Point::Point<Type>
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2U, Vector2F, Vector2
#include "maths/vector2.tpp"             // for Vector2::operator=, operator+

namespace tile
{
    Tile::Tile() : Tile { math::Vector2F { 0, 0 }, math::Vector2F { 0, 0 } } {}

    Tile::Tile( tile::Position position, tile::Position value )
      : Tile { position.pixel().to_float(), value.pixel().to_float() }
    {}

    Tile::Tile( math::Vector2F pixelPosition, math::Vector2F pixelValue )
      : m_quad {}
    {
        this->set_position( pixelPosition );
        this->set_value( pixelValue );
    }

    void Tile::render( RenderContext context ) const
    {
        context.draw( std::vector< sf::Vertex > { m_quad.vertices.begin(),
                                                  m_quad.vertices.end() },
                      sf::Quads );
    }

    math::Vector2F Tile::get_value() const
    {
        return m_quad.set_texture_coord();
    }

    void Tile::set_value( math::Vector2F position )
    {
        m_quad.set_texture_coord(
            math::RectangleF { position, TILE_PIXEL_SIZE_VECTOR } );
    }

    void Tile::set_value( tile::Position position )
    {
        this->set_value( position.pixel().to_float() );
    }

    math::Vector2F Tile::get_position() const
    {
        return m_quad.get_position();
    }

    void Tile::set_position( math::Vector2F position )
    {
        m_quad.set_position(
            math::RectangleF { position, TILE_PIXEL_SIZE_VECTOR } );
    }

    void Tile::set_position( tile::Position position )
    {
        this->set_position( position.pixel().to_float() );
    }

    std::string Tile::get_debug() const
    {
        std::ostringstream outputStream {};
        outputStream << "Tile Position: " << this->get_position() << "\n"
                     << "Vertices Position ( "
                     << math::Vector2F { m_quad.vertices[0].position } << ", "
                     << math::Vector2F { m_quad.vertices[1].position } << ", "
                     << math::Vector2F { m_quad.vertices[2].position } << ", "
                     << math::Vector2F { m_quad.vertices[3].position } << " )"
                     << "\n"
                     << "Tile Value: " << this->get_value() << "\n"
                     << "Vertices TextCoord ( "
                     << math::Vector2F { m_quad.vertices[0].texCoords } << ", "
                     << math::Vector2F { m_quad.vertices[1].texCoords } << ", "
                     << math::Vector2F { m_quad.vertices[2].texCoords } << ", "
                     << math::Vector2F { m_quad.vertices[3].texCoords } << " )";
        return outputStream.str();
    }

    std::ostream & operator<< ( std::ostream & stream, Tile const & tile )
    {
        return stream << tile.get_value();
    }

    std::istream & operator>> ( std::istream & stream, Tile & tile )
    {
        math::Vector2F value;

        stream >> value;

        tile.set_value( value );

        return stream;
    }

    // Tile::Tile()
    //   : Component {},
    //     mp_tilemap { nullptr },
    //     mp_tileset { nullptr },
    //     m_quadVertex {}
    // {}

    // Tile::Tile( tile::Map const & tilemap, Set const & tileset )
    //   : Component {},
    //     mp_tilemap { &tilemap },
    //     mp_tileset { &tileset },
    //     m_quadVertex {}
    // {
    //     this->set_tileset_position( tile::Position {
    //         {0u, 0u},
    //         mp_tileset->get_size(), tile::Position::Tile
    //     } );

    // this->set_tilemap_position( tile::Position {
    //     {0u, 0u},
    //     tilemap.get_size(), tile::Position::Tile
    // } );
    // }

    // Tile::Tile( Tile const & tile ) noexcept
    //   : Component {},
    //     mp_tilemap { tile.mp_tilemap },
    //     mp_tileset { tile.mp_tileset },
    //     m_quadVertex { tile.m_quadVertex }
    // {}

    // Tile::Tile( Tile && tile ) noexcept
    //   : Component {},
    //     mp_tilemap { tile.mp_tilemap },
    //     mp_tileset { tile.mp_tileset },
    //     m_quadVertex { std::exchange( tile.m_quadVertex, {} ) }
    // {}

    // Tile & Tile::operator= ( Tile const & tile ) noexcept
    // {
    //     return *this = Tile { tile };
    // }

    // Tile & Tile::operator= ( Tile && tile ) noexcept
    // {
    //     std::swap( m_quadVertex, tile.m_quadVertex );
    //     return *this;
    // }

    // void Tile::render( RenderContext context ) const
    // {
    //     render.set_texture( mp_tileset->get_texture() );
    //     render.draw( this->get_vertex_array() );
    // }

    // tile::Set const & Tile::get_tileset() const
    // {
    //     return *mp_tileset;
    // }

    // sf::VertexArray const & Tile::get_vertex_array() const
    // {
    //     return m_quadVertex.array;
    // }

    // tile::Position Tile::get_tilemap_position() const
    // {
    //     return tile::Position {
    //         ( m_quadVertex.get_position()
    //           - math::Vector2F { mp_tilemap->getPosition() } )
    //             .to_u_int(),
    //         mp_tilemap->get_size().tile().x, tile::Position::Pixel };
    // }

    // tile::Position Tile::get_tileset_position() const
    // {
    //     return tile::Position {
    //         m_quadVertex.set_texture_coord().to_u_int(),
    //         mp_tileset->get_size().tile().x, tile::Position::Pixel };
    // }

    // void Tile::set_tilemap( tile::Map const & tileMap )
    // {
    //     mp_tilemap = &tileMap;
    // }

    // void Tile::set_tileset( tile::Set const & tileSet )
    // {
    //     mp_tileset = &tileSet;
    // }

    // void Tile::set_tilemap_position( tile::Position position )
    // {
    //     if ( ! mp_tilemap->is_comptatible( position ) )
    //     {
    //         std::cerr << "Tilemap not comptatible with position" <<
    //         std::endl;
    //     }

    // this->set_tilemap_position(
    //     position.pixel() + math::Vector2U { mp_tilemap->getPosition() }
    //     );
    // }

    // void Tile::set_tilemap_position( math::Vector2U pixelPosition )
    // {
    //     math::RectangleF positionRectangle {};

    // positionRectangle.position = pixelPosition.to_float();
    // positionRectangle.size     = TILE_PIXEL_SIZE_VECTOR;

    // m_quadVertex.set_position( positionRectangle );
    // }

    // void Tile::set_tileset_position( tile::Position position )
    // {
    //     /// @todo have the is_compatible function on tileset (like
    //     tilemap or in
    //     /// the set_tilemap_position)

    // this->set_tileset_position( position.pixel() );
    // }

    // void Tile::set_tileset_position( math::Vector2U pixelPosition )
    // {
    //     math::RectangleF textureTileRectangle {};

    // textureTileRectangle.position = pixelPosition.to_float();
    // textureTileRectangle.size     = TILE_PIXEL_SIZE_VECTOR;

    // m_quadVertex.set_texture_coord( textureTileRectangle );
    // }

    // std::string Tile::get_debug() const
    // {
    //     std::ostringstream outputStream {};
    //     outputStream
    //         << "Tile : " << this->get_tileset_position().value() << "\n"
    //         << "Position ( "
    //         << math::Vector2F { m_quadVertex.array[0].position } << ", "
    //         << math::Vector2F { m_quadVertex.array[1].position } << ", "
    //         << math::Vector2F { m_quadVertex.array[2].position } << ", "
    //         << math::Vector2F { m_quadVertex.array[3].position } << " )"
    //         << "\n"
    //         << "TextCoord ( "
    //         << math::Vector2F { m_quadVertex.array[0].texCoords } << ", "
    //         << math::Vector2F { m_quadVertex.array[1].texCoords } << ", "
    //         << math::Vector2F { m_quadVertex.array[2].texCoords } << ", "
    //         << math::Vector2F { m_quadVertex.array[3].texCoords } << "
    //         )";

    // return outputStream.str();
    // }

    // std::ostream & operator<< ( std::ostream & stream, Tile const & tile
    // )
    // {
    //     return stream << tile.get_tilemap_position().tile() << " "
    //                   << tile.get_tileset_position().tile();
    // }

    // std::istream & operator>> ( std::istream & stream, Tile & tile )
    // {
    //     math::Vector2U tilemapPosition, tilesetPosition;

    // stream >> tilemapPosition >> tilesetPosition;

    // tile.set_tilemap_position( tilemapPosition * TILE_PIXEL_SIZE_U );
    // tile.set_tileset_position( tilesetPosition * TILE_PIXEL_SIZE_U );

    // return stream;
    // }
}  // namespace tile