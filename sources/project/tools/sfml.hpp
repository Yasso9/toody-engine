#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#pragma GCC diagnostic pop

/// @brief Improve readability of the use of SFML Library
namespace sfpp
{
    /** @brief Convert a sf::Vector2u to a sf::VideoMode */
    sf::VideoMode to_video_mode( sf::Vector2u const & vector2u ) noexcept;

    /** @brief Get the actual size of the program window */
    sf::Vector2u screen_resolution_u() noexcept;

    /** @brief Get the actual size of the program window */
    sf::Vector2f screen_resolution_f() noexcept;

    sf::Vector2f make_vector( float const & value ) noexcept;
    sf::Vector2f make_vector( unsigned int const & value ) noexcept;
} // namespace sfpp

/** @brief Allow to divide 2 vector */
sf::Vector2f operator/( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB ) noexcept;
/** @brief Allow to multiply 2 vector */
sf::Vector2f operator*( sf::Vector2f const & vectorA,
                        sf::Vector2f const & vectorB ) noexcept;

sf::Vector2f operator%( sf::Vector2f const & vector,
                        int const & modulo ) noexcept;
