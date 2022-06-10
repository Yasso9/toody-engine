#pragma once

#include <cmath>

#include "tools/assertion.hpp"

#include "matrix.hpp"
#include "trigonometry.hpp"
#include "vector2.hpp"

namespace math
{
    /// @todo en faire un template
    class Vector3D : public Vector2< float >
    {
      public:
        float z { 0.f };

        Vector3D() = default;
        Vector3D( float const & xAxisValue, float const & yAxisValue,
                  float const & zAxisValue )
          : Vector2< float >( xAxisValue, yAxisValue ), z( zAxisValue )
        {
            this->normalize();
        }
        virtual ~Vector3D() = default;

        float get_length() const
        {
            return std::sqrt( ( this->x * this->x ) + ( this->y * this->y )
                              + ( this->z * this->z ) );
        }
        Vector3D get_direction() const { return { this->x, this->y, this->z }; }
        void set_direction( Vector3D const & direction )
        {
            this->x = direction.x;
            this->y = direction.y;
            this->z = direction.z;

            this->normalize();
        }

        virtual float operator[]( std::size_t index ) const override
        {
            switch ( index )
            {
            case 0 :
                return this->x;
                break;
            case 1 :
                return this->y;
                break;
            case 2 :
                return this->z;
                break;
            default :
                ASSERTION( false, "Vector3D index out of range" );
                return 0.f;
                break;
            }
        }

        void normalize()
        {
            float length { this->get_length() };
            this->x /= length;
            this->y /= length;
            this->z /= length;
        }
        void rotate( float const & angle, Vector3D const & axisRotation )
        {
            ASSERTION( angle >= -180.f && angle <= 180.f,
                       "An angle must have a value between 0 and 180"s );

            Vector3D const qValue { math::cosinus_degree( angle )
                                    + math::sinus_degree( angle )
                                          * axisRotation };
            Vector3D const qInvertValue { math::power( qValue, -1 ) };

            // Return the quaternions rotation
            this->set_direction( qValue * this->get_direction()
                                 * qInvertValue );
        }
    };

    Vector3D operator*( Vector3D const & vectorLeft,
                        Vector3D const & vectorRight )
    {
        Vector3D result {};

        Matrix2 withoutX {
            {vectorLeft.y, vectorRight.y},
            {vectorLeft.z, vectorRight.z}
        };
        result.x = withoutX.get_determining_factor();
        Matrix2 withoutY {
            {vectorLeft.x, vectorRight.x},
            {vectorLeft.z, vectorRight.z}
        };
        result.y = -withoutY.get_determining_factor();
        Matrix2 withoutZ {
            {vectorLeft.x, vectorRight.x},
            {vectorLeft.y, vectorRight.y}
        };
        result.z = withoutZ.get_determining_factor();

        return result;
    }

    Vector3D operator*( float const & factor, Vector3D const & vector )
    {
        return { vector.x * factor, vector.y * factor, vector.z * factor };
    }
    Vector3D operator/( float const & factor, Vector3D const & vector )
    {
        return { factor / vector.x, factor / vector.y, factor / vector.z };
    }
    Vector3D operator+( float const & summand, Vector3D const & vector )
    {
        return { vector.x + summand, vector.y + summand, vector.z + summand };
    }
    Vector3D power( Vector3D const & vector, int const & powerFactor )
    {
        if ( powerFactor == 0 )
        {
            return { 1.f, 1.f, 1.f };
        }

        Vector3D result { vector };
        for ( unsigned int i_counter { 2u };
              i_counter
              <= static_cast< unsigned int >( std::abs( powerFactor ) );
              ++i_counter )
        {
            result = result * vector;
        }

        if ( powerFactor < 0 )
        {
            result = 1.f / vector;
        }

        return result;
    }
    std::ostream & operator<<( std::ostream & stream, Vector3D const & vector )
    {
        return stream << "( " << vector.x << ", " << vector.y << ", "
                      << vector.z << " )";
    }
} // namespace math