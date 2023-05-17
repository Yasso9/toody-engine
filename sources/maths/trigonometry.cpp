#include "trigonometry.hpp"

#include <cmath>    // for cos, sin
#include <numbers>  // for pi

namespace math
{
    float cosinus_radian ( float const & radian )
    {
        return std::cos( radian );
    }

    float cosinus_degree ( float const & degree )
    {
        return cosinus_radian( degree_to_radian( degree ) );
    }

    float sinus_radian ( float const & radian )
    {
        return std::sin( radian );
    }

    float sinus_degree ( float const & degree )
    {
        return sinus_radian( degree_to_radian( degree ) );
    }

    float degree_to_radian ( float const & degree )
    {
        // radian = degree * π / 180
        return degree * static_cast< float >( std::numbers::pi / 180. );
    }

    float radian_to_degree ( float const & radian )
    {
        // degree = radian * 180 / π
        return radian * static_cast< float >( 180. / std::numbers::pi );
    }
}  // namespace math