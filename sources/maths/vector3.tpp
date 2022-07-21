#pragma once

#include "vector3.hpp"

namespace math
{
    template < C_Primitive Type >
    void Vector3< Type >::rotate( float angle,
                                  Vector3< Type > /* axisRotation */ )
    {
        ASSERTION( angle >= -180.f && angle <= 180.f,
                   "An angle must have a value between 0 and 180"s );

        // Vector3 const qValue { math::cosinus_degree( angle )
        //                        + math::sinus_degree( angle ) * axisRotation };
        // Vector3 const qInvertValue { math::power( qValue, -1 ) };

        // Return the quaternions rotation
        // this->set_direction( qValue * this->get_direction() * qInvertValue );
    }

    /* ************************************************************************
    ************************** VECTOR X VECTOR ********************************
    ************************************************************************ */

    template < C_Primitive Type >
    Vector3< Type > operator*( Vector3< Type > vectorLeft,
                               Vector3< Type > vectorRight )
    {
        Vector3< Type > result {};

        Matrix2 withoutX {
            {vectorLeft.y,  vectorRight.y},
            { vectorLeft.z, vectorRight.z}
        };
        result.x = withoutX.get_determining_factor();
        Matrix2 withoutY {
            {vectorLeft.x,  vectorRight.x},
            { vectorLeft.z, vectorRight.z}
        };
        result.y = -withoutY.get_determining_factor();
        Matrix2 withoutZ {
            {vectorLeft.x,  vectorRight.x},
            { vectorLeft.y, vectorRight.y}
        };
        result.z = withoutZ.get_determining_factor();

        return result;
    }

    /* ************************************************************************
    ************************** VECTOR X FACTOR ********************************
    ************************************************************************ */

    template < C_Primitive Type >
    Vector3< Type > operator*( Vector3< Type > vector, float factor )
    {
        return { vector.x * factor, vector.y * factor, vector.z * factor };
    }
    template < C_Primitive Type >
    Vector3< Type > operator/( Vector3< Type > vector, float factor )
    {
        return { factor / vector.x, factor / vector.y, factor / vector.z };
    }
    template < C_Primitive Type >
    Vector3< Type > power( Vector3< Type > vector, int powerFactor )
    {
        if ( powerFactor == 0 )
        {
            return { 1.f, 1.f, 1.f };
        }

        Vector3< Type > result { vector };
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

    /* ************************************************************************
    ************************** VECTOR X OTHERS ********************************
    ************************************************************************ */

    template < C_Primitive Type >
    std::ostream & operator<<( std::ostream & stream, Vector3< Type > vector )
    {
        return stream << "( " << vector.x << ", " << vector.y << ", "
                      << vector.z << " )";
    }
} // namespace math