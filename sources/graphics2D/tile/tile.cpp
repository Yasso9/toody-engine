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

    void Tile::render( RenderContext & context ) const
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
}  // namespace tile
