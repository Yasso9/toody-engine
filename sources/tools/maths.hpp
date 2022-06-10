#pragma once

#include <array>
#include <cmath>

#include "graphics2D/sfml.hpp"
#include "tools/assertion.hpp"
#include "tools/imgui.hpp"

namespace math
{
    struct Rectangle;

    float cosinus_radian( float const & value );
    float cosinus_degree( float const & value );
    float sinus_radian( float const & value );
    float sinus_degree( float const & value );

    float degree_to_radian( float const & degree );
    float radian_to_degree( float const & radian );

    /**
     * @brief Check if a vector is between the position and position + size
     */
    template < typename Type >
    bool is_inside( Vector2< Type > const & value,
                    Vector2< Type > const & position,
                    Vector2< Type > const & size );

    /// @todo en faire un template
    struct Rectangle
    {
        Vector2< float > position { 0.f, 0.f };
        Vector2< float > size { 0.f, 0.f };

        Rectangle() = default;
        Rectangle( Vector2< float > const & aPosition,
                   Vector2< float > const & aSize );
        Rectangle( float const & x, float const & y, float const & width,
                   float const & height );
        virtual ~Rectangle() = default;
    };
} // namespace math