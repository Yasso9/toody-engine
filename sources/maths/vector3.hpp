#pragma once

#include <cmath>

#include "tools/assertion.hpp"

#include "matrix.hpp"
#include "trigonometry.hpp"
#include "vector2.hpp"

namespace math
{
    class Vector3D : public Vector2< float >
    {
      public:
        float z { 0.f };

        Vector3D() = default;
        Vector3D( float const & xAxisValue, float const & yAxisValue,
                  float const & zAxisValue );
        virtual ~Vector3D() = default;

        float get_length() const;
        Vector3D get_direction() const;
        void set_direction( Vector3D const & direction );

        virtual float operator[]( std::size_t index ) const override;

        void normalize();
        void rotate( float const & angle, Vector3D const & axisRotation );
    };

    Vector3D operator*( Vector3D const & vectorLeft,
                        Vector3D const & vectorRight );

    Vector3D operator*( float const & factor, Vector3D const & vector );
    Vector3D operator/( float const & factor, Vector3D const & vector );
    Vector3D operator+( float const & summand, Vector3D const & vector );
    Vector3D power( Vector3D const & vector, int const & powerFactor );
    std::ostream & operator<<( std::ostream & stream, Vector3D const & vector );
} // namespace math