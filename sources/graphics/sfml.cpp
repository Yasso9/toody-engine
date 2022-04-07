#include "sfml.hpp"

namespace sfpp
{
    sf::VideoMode to_video_mode( sf::Vector2u const & vector2u )
    {
        return sf::VideoMode { vector2u.x, vector2u.y };
    }

    sf::Vector2u screen_resolution_u()
    {
        sf::VideoMode const videoMode { sf::VideoMode::getDesktopMode() };
        return sf::Vector2u { videoMode.width, videoMode.height };
    }

    sf::Vector2f screen_resolution_f()
    {
        return static_cast< sf::Vector2f >( sfpp::screen_resolution_u() );
    }

    sf::Vector2f make_vector( float const & value )
    {
        return sf::Vector2f { value, value };
    }

    sf::Vector2f make_vector( unsigned int const & value )
    {
        return sfpp::make_vector( static_cast< float >( value ) );
    }
} // namespace sfpp

sf::Vector2f operator/( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB )
{
    return sf::Vector2f { vectorA.x / vectorB.x, vectorA.y / vectorB.y };
}

sf::Vector2f operator*( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB )
{
    return sf::Vector2f { vectorA.x * vectorB.x, vectorA.y * vectorB.y };
}

sf::Vector2f operator%( sf::Vector2f const & vector, int const & modulo )
{
    sf::Vector2i const integerVector { static_cast< sf::Vector2i >( vector ) };

    sf::Vector2i const integerVectorResult { integerVector.x % modulo,
                                             integerVector.y % modulo };

    return static_cast< sf::Vector2f >( integerVectorResult );
}

std::ostream & operator<<( std::ostream & stream,
                           sf::ContextSettings const & settings )
{
    stream << "{  depth bits : " << settings.depthBits << " , ";
    stream << "stencil bits : " << settings.stencilBits << " , ";
    stream << "sRgbCapable : " << settings.sRgbCapable << " , ";
    stream << "antialiasing level : " << settings.antialiasingLevel << " , ";
    stream << "version : " << settings.majorVersion << "."
           << settings.minorVersion << "  }";

    return stream;
}

template < typename T >
std::ostream & operator<<( std::ostream & stream, sf::Rect< T > const & rect )
{
    return stream << "( " << rect.left << ", " << rect.top << ", " << rect.width
                  << ", " << rect.height << " )";
}
template std::ostream & operator<<( std::ostream & stream,
                                    sf::Rect< int > const & rect );
template std::ostream & operator<<( std::ostream & stream,
                                    sf::Rect< float > const & rect );

template < typename T >
std::ostream & operator<<( std::ostream & stream,
                           sf::Vector2< T > const & vector )
{
    return stream << "( " << vector.x << ", " << vector.y << " )";
}
template std::ostream & operator<<( std::ostream & stream,
                                    sf::Vector2< float > const & vector );
template std::ostream & operator<<(
    std::ostream & stream, sf::Vector2< unsigned int > const & vector );
template std::ostream & operator<<( std::ostream & stream,
                                    sf::Vector2< int > const & vector );