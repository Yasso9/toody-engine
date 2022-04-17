#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wlanguage-extension-token"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#pragma GCC diagnostic pop

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

template < typename T >
std::ostream & operator<<( std::ostream & stream, sf::Rect< T > const & rect );
template < typename T >
std::ostream & operator<<( std::ostream & stream,
                           sf::Vector2< T > const & vector );