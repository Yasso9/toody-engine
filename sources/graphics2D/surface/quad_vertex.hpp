#pragma once

#include <array>

#include <SFML/Graphics/VertexArray.hpp>  // for VertexArray

#include "maths/geometry/rect.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F

class QuadVertex
{
  public:
    std::array< sf::Vertex, 4 > vertices;

    QuadVertex();
    virtual ~QuadVertex() = default;

    math::Vector2F get_position () const;
    math::Vector2F set_texture_coord () const;

    /// @brief set vertex array position and size
    void set_position ( math::RectangleF const & rectangle );
    void set_texture_coord ( math::RectangleF const & rectangle );
};
