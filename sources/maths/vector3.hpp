#pragma once

#include <cmath>
#include <ostream>

#include <glm/vec3.hpp>

#include "maths/matrix.hpp"
#include "maths/trigonometry.hpp"
#include "tools/assertion.hpp"
#include "tools/concepts.hpp"

namespace math
{
    template< cPrimitive Type >
    class Vector3;
    using Vector3F = Vector3< float >;
    using Vector3I = Vector3< int >;
    using Vector3U = Vector3< unsigned int >;

    template< cPrimitive Type >
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

        Vector3( glm::vec3 const & glmVector )
            requires ( std::is_same_v< Type, float > )
          : Vector3( glmVector.x, glmVector.y, glmVector.z )
        {}

        virtual ~Vector3() = default;

        glm::vec3 to_glm () const requires ( std::is_same_v< Type, float > )
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

        /* ********************************************************************
        ************************ ASSIGNEMENT OPERATOR *************************
        ******************************************************************** */

        Vector3< Type > operator-= ( Vector3< Type > lhs )
        {
            return *this = *this - lhs;
        }

        Vector3< Type > operator+= ( Vector3< Type > lhs )
        {
            return *this = *this + lhs;
        }

        template< cPrimitive OtherType >
        Vector3< Type > operator*= ( OtherType factorLHS )
        {
            return *this = *this * factorLHS;
        }

        float get_length () const
        {
            return std::sqrt( ( this->x * this->x ) + ( this->y * this->y )
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
                throw std::out_of_range { "Vector3 index out of range" };
                break;
            }
        }
    };

    /* ************************************************************************
    ***************************** FUNCTIONS ***********************************
    ************************************************************************ */

    template< cPrimitive Type >
    Vector3< Type > normalize ( Vector3< Type > vector );

    /* ************************************************************************
    ************************** VECTOR X VECTOR ********************************
    ************************************************************************ */

    template< cPrimitive Type >
    Vector3< Type > operator* ( Vector3< Type > lhs, Vector3< Type > rhs );
    template< cPrimitive Type >
    Vector3< Type > operator+ ( Vector3< Type > lhs, Vector3< Type > rhs );
    template< cPrimitive Type >
    Vector3< Type > operator- ( Vector3< Type > lhs, Vector3< Type > rhs );

    /* ************************************************************************
    ************************** VECTOR X FACTOR ********************************
    ************************************************************************ */

    template< cPrimitive Type >
    Vector3< Type > operator* ( Vector3< Type > vector, Type factor );
    template< cPrimitive Type >
    Vector3< Type > operator/ ( Vector3< Type > vector, Type factor );
    template< cPrimitive Type >
    Vector3< Type > power ( Vector3< Type > vector, int powerFactor );

    /* ************************************************************************
    ************************** VECTOR X OTHERS ********************************
    ************************************************************************ */

    template< cPrimitive Type >
    std::ostream & operator<< ( std::ostream & stream, Vector3< Type > vector );
}  // namespace math

#include "vector3.tpp"