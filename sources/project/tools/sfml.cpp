#include "sfml.hpp"

namespace sfpp
{
    sf::VideoMode to_video_mode( sf::Vector2u const & vector2u ) noexcept
    {
        return sf::VideoMode { vector2u.x, vector2u.y };
    }

    sf::Vector2u screen_resolution_u() noexcept
    {
        sf::VideoMode const videoMode { sf::VideoMode::getDesktopMode() };
        return sf::Vector2u { videoMode.width, videoMode.height };
    }

    sf::Vector2f screen_resolution_f() noexcept
    {
        return static_cast<sf::Vector2f>( sfpp::screen_resolution_u() );
    }

    sf::Vector2f make_vector( float const & value ) noexcept
    {
        return sf::Vector2f { value, value };
    }

    sf::Vector2f make_vector( unsigned int const & value ) noexcept
    {
        return sfpp::make_vector( static_cast<float>( value ) );
    }
} // namespace sfpp

sf::Vector2f operator/( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB ) noexcept
{
    return sf::Vector2f { vectorA.x / vectorB.x, vectorA.y / vectorB.y };
}

sf::Vector2f operator*( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB ) noexcept
{
    return sf::Vector2f { vectorA.x * vectorB.x, vectorA.y * vectorB.y };
}

sf::Vector2f operator%( sf::Vector2f const & vector,
                        int const & modulo ) noexcept
{
    sf::Vector2i const integerVector { static_cast<sf::Vector2i>( vector ) };

    sf::Vector2i const integerVectorResult { integerVector.x % modulo,
                                             integerVector.y % modulo };

    return static_cast<sf::Vector2f>( integerVectorResult );
}