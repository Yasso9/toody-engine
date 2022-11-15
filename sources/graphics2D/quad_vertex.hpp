#pragma once

#include <SFML/Graphics/VertexArray.hpp>  // for VertexArray

#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F

struct QuadVertex
{
    sf::VertexArray array;

    QuadVertex();
    virtual ~QuadVertex() = default;

    math::Vector2F get_position () const;
    math::Vector2F get_texture_position () const;

    /// @brief set vertex array position and size
    void set_position ( math::RectangleF const & rectangle );
    void set_texture_coord ( math::RectangleF const & rectangle );
};