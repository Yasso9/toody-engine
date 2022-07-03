#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

struct QuadVertex
{
    sf::VertexArray array;

    QuadVertex();

    math::Vector2F get_position() const;
    math::Vector2F get_texture_position() const;

    /// @brief set vertex array position and size
    void set_position( math::RectangleF const & rectangle );
    void set_texture_coord( math::RectangleF const & rectangle );
};