#pragma once

#include <project/utility/utility.hpp>

/* Prend en parametre la position d'un nombre donné en pixel et retourne sa position
 * en tile */
sf::Vector2u to_tile_position( unsigned int const tileNumber,
                               unsigned int const numberOfTileInX );

// Surcharge de la dernière fonction
/* Prend en parametre la position d'un nombre donné en pixel et retourne sa position
 * en tile */
sf::Vector2u to_tile_position( sf::Vector2f const position );

void set_quad_position( sf::VertexArray & quad,
                        sf::Vector2f const & basePosition,
                        sf::Vector2u const & tilePosition );

void set_quad_texture_coordinate( sf::VertexArray & quad,
                                  sf::Vector2u const & tilePosition );

void set_quad_transparency( sf::VertexArray & quad );

/* Rend visible le carré du quad (couleur noire) */
void set_quad_visible( sf::VertexArray & quad );