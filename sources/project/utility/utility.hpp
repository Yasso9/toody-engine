#pragma once

// To turn off warning that are caused by the sfml
// save diagnostic state
#pragma GCC diagnostic push
// turn off the specific warning. Can also use "-Wall"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wlong-long"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// turn the warnings back on
#pragma GCC diagnostic pop

// Pour les states
#include <stack>

// smart pointer
#include <memory>

// Calcul dans la tilemap (std::floor)
#include <cmath>

// Tableau
#include <array>
#include <vector>

// Errors and exception handling
#include <cassert>
#include <exception>
#include <stdexcept>

// String and stream manipulation
#include <charconv>
#include <fstream>
#include <sstream>
#include <string>

// A enlever a la fin (peut être)
#include <iostream>
// Pour les pair (make_pair quand on initialise les valeurs des inputs)
#include <utility>

using namespace std::string_literals;

constexpr float const g_framerate { 60 };

/// @brief size of the a tile sqare in pixel
constexpr float const g_squareSize { 32 };

// TYPO a enlever, peut se calculer automatiquement en comptant le nb d'instance
// d'enfant de State
constexpr int const g_numberOfState { 3 };

#include <project/tilemap/tile_utility.hpp>
#include <project/tools/enumeration.hpp>
#include <project/utility/error.hpp>

void secure_entry( unsigned int & size );

// Surchage pour facilité le debugage, à enlever peut être a la fin du programme
std::ostream & operator<<( std::ostream & flux, sf::Vector2f const & vector );
// Surchage pour facilité le debugage, à enlever peut être a la fin du programme
std::ostream & operator<<( std::ostream & flux, sf::Vector2u const & vector );

// Retourne le movemment que la player doit faire en fonction de la direction et
// d'une vitesse donné (si il cours ou pas)
sf::Vector2f get_movement( Direction const direction, float const value );

/* Convertie une position lié à la lastView à une position lié à une newView
 * donnée
 */
sf::Vector2f to_view( sf::Vector2f const & position, sf::View const & lastView,
                      sf::View const & newView );