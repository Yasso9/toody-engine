#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/vector2.hpp"

struct QuadVertex
{
    sf::VertexArray array;

    QuadVertex() : array( sf::Quads, 4 ) {};

    /// @brief set vertex array position and size
    void set_position( math::RectangleF const & rectangle )
    {
        this->array[0].position = rectangle.position;
        this->array[1].position =
            rectangle.position + math::Vector2F { rectangle.size.x, 0.f };
        this->array[2].position = rectangle.position + rectangle.size;
        this->array[3].position =
            rectangle.position + math::Vector2F { 0.f, rectangle.size.y };
    }
    void set_texture_coord( math::RectangleF const & rectangle )
    {
        this->array[0].texCoords = rectangle.position;
        this->array[1].texCoords =
            rectangle.position + math::Vector2F { rectangle.size.x, 0.f };
        this->array[2].texCoords = rectangle.position + rectangle.size;
        this->array[3].texCoords =
            rectangle.position + math::Vector2F { 0.f, rectangle.size.y };
    }
};