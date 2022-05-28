#include "maths.hpp"

#include <numbers>

namespace math
{
    float cosinus_radian( float const & radian ) { return std::cos( radian ); }
    float cosinus_degree( float const & degree )
    {
        return cosinus_radian( degree_to_radian( degree ) );
    }
    float sinus_radian( float const & radian ) { return std::sin( radian ); }
    float sinus_degree( float const & degree )
    {
        return sinus_radian( degree_to_radian( degree ) );
    }

    float degree_to_radian( float const & degree )
    {
        // radian = degree * π / 180
        return degree * static_cast< float >( std::numbers::pi / 180. );
    }
    float radian_to_degree( float const & radian )
    {
        // degree = radian * 180 / π
        return radian * static_cast< float >( 180. / std::numbers::pi );
    }

    /* ************************************************************************************** */
    /* ***********************************  Rectangle  ***************************************** */
    /* ************************************************************************************** */

    Rectangle::Rectangle( Vector2D const & aPosition, Vector2D const & aSize )
      : position( aPosition ), size( aSize )
    {}
    Rectangle::Rectangle( float const & x, float const & y, float const & width,
                          float const & height )
      : position( x, y ), size( width, height )
    {}

    /* ************************************************************************************** */
    /* ***********************************  VECTOR  ***************************************** */
    /* ************************************************************************************** */

    Vector2D::Vector2D( float const & xAxisValue, float const & yAxisValue )
      : x( xAxisValue ), y( yAxisValue )
    {}

    Vector2D::Vector2D( sf::Vector2f const & sfmlVector )
      : x( sfmlVector.x ), y( sfmlVector.y )
    {}

    float Vector2D::operator[]( std::size_t index ) const
    {
        switch ( index )
        {
        case 0 :
            return this->x;
            break;
        case 1 :
            return this->y;
            break;
        default :
            ASSERTION( false, "Vector index out of range" );
            break;
        }

        // TYPO must return a falsy variable
        return this->x;
    }

    bool Vector2D::is_contained( Rectangle const & rectangle )
    {
        return math::is_contained( *this, rectangle.position, rectangle.size );
    }
    bool Vector2D::is_contained( Vector2D const & position,
                                 Vector2D const & size )
    {
        return math::is_contained( *this, position, size );
    }

    bool is_contained( Vector2D const & value, Vector2D const & position,
                       Vector2D const & size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }

    /* ************************************************************************************** */
    /* ***********************************  VECTOR  ***************************************** */
    /* ************************************************************************************** */

    Vector3D::Vector3D( float const & xAxisValue, float const & yAxisValue,
                        float const & zAxisValue )
      : Vector2D( xAxisValue, yAxisValue ), z( zAxisValue )
    {
        this->normalize();
    }

    Vector3D Vector3D::get_direction() const
    {
        return { this->x, this->y, this->z };
    }
    float Vector3D::get_length() const
    {
        return std::sqrt( ( this->x * this->x ) + ( this->y * this->y )
                          + ( this->z * this->z ) );
    }
    void Vector3D::set_direction( Vector3D const & direction )
    {
        this->x = direction.x;
        this->y = direction.y;
        this->z = direction.z;

        this->normalize();
    }

    float Vector3D::operator[]( std::size_t index ) const
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
            ASSERTION( false, "Vector index out of range" );
            break;
        }

        // TYPO must return a falsy variable
        return this->x;
    }

    void Vector3D::normalize()
    {
        float length { this->get_length() };
        this->x /= length;
        this->y /= length;
        this->z /= length;
    }
    void Vector3D::rotate( float const & angle, Vector3D const & axisRotation )
    {
        ASSERTION( angle >= -180.f && angle <= 180.f,
                   "An angle must have a value between 0 and 180"s );

        Vector3D const qValue { math::cosinus_degree( angle )
                                + math::sinus_degree( angle ) * axisRotation };
        Vector3D const qInvertValue { math::power( qValue, -1 ) };

        // Return the quaternions rotation
        this->set_direction( qValue * this->get_direction() * qInvertValue );
    }

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

    Vector3D operator/( float const & factor, Vector3D const & vector )
    {
        return { factor / vector.x, factor / vector.y, factor / vector.z };
    }
    Vector3D operator*( float const & factor, Vector3D const & vector )
    {
        return { vector.x * factor, vector.y * factor, vector.z * factor };
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

    /* ************************************************************************************** */
    /* ***********************************  MATRIX  ***************************************** */
    /* ************************************************************************************** */

    Matrix2::Matrix2( Vector2D const & vectorLine1,
                      Vector2D const & vectorLine2 )
      : m_vectorArray( { vectorLine1, vectorLine2 } )
    {}

    Vector2D Matrix2::operator[]( size_t index ) const
    {
        return m_vectorArray[index];
    }
    float Matrix2::get_determining_factor() const
    {
        return ( this->m_vectorArray[0][0] * this->m_vectorArray[1][1] )
               - ( this->m_vectorArray[0][1] * this->m_vectorArray[1][0] );
    }
} // namespace math