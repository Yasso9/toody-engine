#include "quad_vertex.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>  // for Quads
#include <SFML/Graphics/Vertex.hpp>         // for Vertex
#include <SFML/System/Vector2.hpp>          // for Vector2f

#include "maths/geometry/point.hpp"  // for Point
#include "maths/vector2.tpp"         // for operator+, Vector2::opera...

QuadVertex::QuadVertex() : array( sf::Quads, 4 ) {};

math::Vector2F QuadVertex::get_position() const
{
    return { this->array[0].position };
}

math::Vector2F QuadVertex::get_texture_position() const
{
    return { this->array[0].texCoords };
}

/// @brief set vertex array position and size
void QuadVertex::set_position( math::RectangleF const & rectangle )
{
    this->array[0].position = rectangle.position;
    this->array[1].position =
        rectangle.position + math::Vector2F { rectangle.size.x, 0.f };
    this->array[2].position = rectangle.position + rectangle.size;
    this->array[3].position =
        rectangle.position + math::Vector2F { 0.f, rectangle.size.y };
}

void QuadVertex::set_texture_coord( math::RectangleF const & rectangle )
{
    this->array[0].texCoords = rectangle.position;
    this->array[1].texCoords =
        rectangle.position + math::Vector2F { rectangle.size.x, 0.f };
    this->array[2].texCoords = rectangle.position + rectangle.size;
    this->array[3].texCoords =
        rectangle.position + math::Vector2F { 0.f, rectangle.size.y };
}