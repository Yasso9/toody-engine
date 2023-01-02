#pragma once

#include "vector3.hpp"

namespace math
{
    /* ************************************************************************
    ***************************** FUNCTIONS ***********************************
    ************************************************************************ */

    template< C_Primitive Type >
    Vector3< Type > normalize ( Vector3< Type > vector )
    {
        float const length { vector.get_length() };
        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
        return vector;
    }

    /* ************************************************************************
    ************************** VECTOR X VECTOR ********************************
    ************************************************************************ */

    template< C_Primitive Type >
    Vector3< Type > operator* ( Vector3< Type > vectorLeft,
                                Vector3< Type > vectorRight )
    {
        Vector3< Type > result {};

        Matrix2 withoutX { { vectorLeft.y, vectorRight.y },
                           { vectorLeft.z, vectorRight.z } };
        result.x = withoutX.get_determining_factor();
        Matrix2 withoutY { { vectorLeft.x, vectorRight.x },
                           { vectorLeft.z, vectorRight.z } };
        result.y = -withoutY.get_determining_factor();
        Matrix2 withoutZ { { vectorLeft.x, vectorRight.x },
                           { vectorLeft.y, vectorRight.y } };
        result.z = withoutZ.get_determining_factor();

        return result;
    }

    template< C_Primitive Type >
    Vector3< Type > operator+ ( Vector3< Type > lhs, Vector3< Type > rhs )
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
    }

    template< C_Primitive Type >
    Vector3< Type > operator- ( Vector3< Type > lhs, Vector3< Type > rhs )
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
    }

    /* ************************************************************************
    ************************** VECTOR X FACTOR ********************************
    ************************************************************************ */

    template< C_Primitive Type >
    Vector3< Type > operator* ( Vector3< Type > vector, Type factor )
    {
        return { vector.x * factor, vector.y * factor, vector.z * factor };
    }

    template< C_Primitive Type >
    Vector3< Type > operator/ ( Vector3< Type > vector, Type factor )
    {
        return { factor / vector.x, factor / vector.y, factor / vector.z };
    }

    template< C_Primitive Type >
    Vector3< Type > power ( Vector3< Type > vector, int powerFactor )
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

    template< C_Primitive Type >
    std::ostream & operator<< ( std::ostream & stream, Vector3< Type > vector )
    {
        return stream << "( " << vector.x << ", " << vector.y << ", "
                      << vector.z << " )";
    }
}  // namespace math