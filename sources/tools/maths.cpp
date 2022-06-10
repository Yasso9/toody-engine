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

    Rectangle::Rectangle( Vector2 const & aPosition, Vector2 const & aSize )
      : position( aPosition ), size( aSize )
    {}
    Rectangle::Rectangle( float const & x, float const & y, float const & width,
                          float const & height )
      : position( x, y ), size( width, height )
    {}

    /* ************************************************************************************** */
    /* ***********************************  VECTOR  ***************************************** */
    /* ************************************************************************************** */

    bool is_inside( Vector2 const & value, Vector2 const & position,
                    Vector2 const & size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }

} // namespace math