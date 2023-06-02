#include "quad_vertex.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>  // for Quads
#include <SFML/Graphics/Vertex.hpp>         // for Vertex
#include <SFML/System/Vector2.hpp>          // for Vector2f

#include "maths/geometry/point.hpp"  // for Point
#include "maths/vector2.tpp"         // for operator+, Vector2::opera...

QuadVertex::QuadVertex() : vertices {} {};

math::Vector2F QuadVertex::get_position() const
{
    return math::Vector2F { this->vertices[0].position };
}

math::Vector2F QuadVertex::set_texture_coord() const
{
    return math::Vector2F { this->vertices[0].texCoords };
}

void QuadVertex::set_position( math::RectangleF const & rectangle )
{
    this->vertices[0].position = rectangle.position;
    this->vertices[1].position = rectangle.position
                                 + math::Vector2F { rectangle.size.x, 0.f };
    this->vertices[2].position = rectangle.position + rectangle.size;
    this->vertices[3].position = rectangle.position
                                 + math::Vector2F { 0.f, rectangle.size.y };
}

void QuadVertex::set_texture_coord( math::RectangleF const & rectangle )
{
    this->vertices[0].texCoords = rectangle.position;
    this->vertices[1].texCoords = rectangle.position
                                  + math::Vector2F { rectangle.size.x, 0.f };
    this->vertices[2].texCoords = rectangle.position + rectangle.size;
    this->vertices[3].texCoords = rectangle.position
                                  + math::Vector2F { 0.f, rectangle.size.y };
}
