#pragma once

#include <cmath>
#include <ostream>

#include <GLM/vec3.hpp>

#include "maths/matrix.hpp"
#include "maths/trigonometry.hpp"
#include "tools/assertion.hpp"
#include "tools/concepts.hpp"

namespace math
{
    template< C_Primitive Type >
    class Vector3;
    using Vector3F = Vector3< float >;
    using Vector3I = Vector3< int >;
    using Vector3U = Vector3< unsigned int >;

    template< C_Primitive Type >
    class Vector3
    {
      public:
        Type x;
        Type y;
        Type z;

        Vector3() : Vector3( 0, 0, 0 ) {}

        Vector3( Type xAxisValue, Type yAxisValue, Type zAxisValue )
          : x { xAxisValue }, y( yAxisValue ), z( zAxisValue )
        {}

        Vector3( glm::vec3 const & glmVector ) requires(
            std::is_same_v< Type, float > )
          : Vector3( glmVector.x, glmVector.y, glmVector.z )
        {}

        virtual ~Vector3() = default;

        glm::vec3 to_glm () const requires( std::is_same_v< Type, float > )
        {
            return glm::vec3 { this->x, this->y, this->z };
        }

        Vector3F to_float () const
        {
            return Vector3F {
                static_cast< float >( this->x ),
                static_cast< float >( this->y ),
                static_cast< float >( this->z ),
            };
        }

        Vector3I to_int () const
        {
            return Vector3I {
                static_cast< int >( this->x ),
                static_cast< int >( this->y ),
                static_cast< int >( this->z ),
            };
        }

        Vector3U to_u_int () const
        {
            return Vector3U {
                static_cast< unsigned int >( this->x ),
                static_cast< unsigned int >( this->y ),
                static_cast< unsigned int >( this->z ),
            };
        }

        float get_length () const
        {
            return std::sqrt(
                ( this->x * this->x ) + ( this->y * this->y )
                + ( this->z * this->z ) );
        }

        virtual float operator[] ( unsigned int index ) const
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
                ASSERTION( false, "Vector3 index out of range" );
                return 0.f;
                break;
            }
        }

        void normalize ()
        {
            float const length { this->get_length() };
            this->x /= length;
            this->y /= length;
            this->z /= length;
        }

        void rotate ( float angle, Vector3< Type > axisRotation );
    };

    /* ************************************************************************
    ************************** VECTOR X VECTOR ********************************
    ************************************************************************ */

    template< C_Primitive Type >
    Vector3< Type > operator* (
        Vector3< Type > vectorLeft, Vector3< Type > vectorRight );

    /* ************************************************************************
    ************************** VECTOR X FACTOR ********************************
    ************************************************************************ */

    template< C_Primitive Type >
    Vector3< Type > operator* ( Vector3< Type > vector, Type factor );
    template< C_Primitive Type >
    Vector3< Type > operator/ ( Vector3< Type > vector, Type factor );
    template< C_Primitive Type >
    Vector3< Type > power ( Vector3< Type > vector, int powerFactor );

    /* ************************************************************************
    ************************** VECTOR X OTHERS ********************************
    ************************************************************************ */

    template< C_Primitive Type >
    std::ostream & operator<< ( std::ostream & stream, Vector3< Type > vector );
}  // namespace math

#include "vector3.tpp"