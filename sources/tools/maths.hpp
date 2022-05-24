#pragma once

#include <array>
#include <cmath>

#include "tools/assertion.hpp"

namespace math
{
    float cosinus_radian( float const & value );
    float cosinus_degree( float const & value );
    float sinus_radian( float const & value );
    float sinus_degree( float const & value );

    float degree_to_radian( float const & degree );
    float radian_to_degree( float const & radian );

    struct Vector2D
    {
        float x { 0.f };
        float y { 0.f };

        Vector2D() = default;
        Vector2D( float const & xAxisValue, float const & yAxisValue );
        virtual ~Vector2D() = default;

        virtual float operator[]( std::size_t index ) const;
    };

    struct Vector3D : public Vector2D
    {
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
        void rotate( float const & angle, Vector3D const & directionAxis );
    };

    Vector3D operator*( Vector3D const & vectorLeft,
                        Vector3D const & vectorRight );

    Vector3D operator*( float const & factor, Vector3D const & vector );
    Vector3D operator/( float const & factor, Vector3D const & vector );
    Vector3D operator+( float const & summand, Vector3D const & vector );
    Vector3D power( Vector3D const & vector, int const & powerFactor );

    std::ostream & operator<<( std::ostream & stream, Vector3D const & vector );

    class Matrix2
    {
      public:
        Matrix2( Vector2D const & vectorLine1, Vector2D const & vectorLine2 );
        virtual ~Matrix2() = default;

        Vector2D operator[]( std::size_t index ) const;
        float get_determining_factor() const;

      private:
        std::array< Vector2D, 2 > m_vectorArray;
    };
} // namespace math