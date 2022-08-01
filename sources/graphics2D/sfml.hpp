#pragma once

#include <iosfwd> // for ostream

#include <SFML/System/Vector2.hpp>   // for Vector2f, Vector2u, Vector2 (pt...
#include <SFML/Window/VideoMode.hpp> // for VideoMode

namespace sf
{
    class Color;
    struct ContextSettings;
} // namespace sf

/// @brief Improve readability of the use of SFML Library
namespace sfpp
{
    /** @brief Convert a sf::Vector2u to a sf::VideoMode */
    sf::VideoMode to_video_mode( sf::Vector2u const & vector2u );

    /** @brief Get the actual size of the program window */
    sf::Vector2u screen_resolution_u();

    /** @brief Get the actual size of the program window */
    sf::Vector2f screen_resolution_f();

    sf::Vector2f make_vector( float const & value );
    sf::Vector2f make_vector( unsigned int const & value );
} // namespace sfpp

/** @brief Allow to divide 2 vector */
sf::Vector2f operator/( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB );
/** @brief Allow to multiply 2 vector */
sf::Vector2f operator*( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB );

sf::Vector2f operator%( sf::Vector2f const & vector, int const & modulo );

std::ostream & operator<<( std::ostream & stream,
                           sf::ContextSettings const & settings );
std::ostream & operator<<( std::ostream & stream, sf::Color const & color );

template < typename T >
std::ostream & operator<<( std::ostream & stream,
                           sf::Vector2< T > const & vector );