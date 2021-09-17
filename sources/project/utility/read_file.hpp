#pragma once

#include "utility.hpp"


/* Etat que peut avoir le curseur de la chaine qui lit le fichier.
NextDefinition et End sont dans les fait la même chose
mais pour des soucis de compréhension on crée deux énumération */
enum FileStep { SameLine = 0, NextLine, NextDefinition, End };

/* Enlève tout les caractères de type espaces dans une chaine */
void erase_space( std::string & text );

/* Retourne toute la chaine de caractère du fichier
sans prendre en considération les espaces */
std::string read_file( std::string const & fileName );

std::string read_definition( std::string const & fileContent, unsigned int & index );

int read_number( std::string const & fileContent, unsigned int & index );

/* Lit une ligne de la tilemap, tout les paremetre retourné sont passé par référence */
void read_tilemap_line( std::string const & fileContent, unsigned int & index,
    std::vector<int> & tilemapTable, sf::Vector2u & verificationSize,
    FileStep & FileStep, bool shouldReadNumber );

/* Verifie qu'il y'a le même nombre de valeur dans soit chaque ligne
ou chaque colonne du fichier */
void vericafication_string_inconsistency( unsigned int const & loopIterator,
    unsigned int & lastValue, unsigned int & newValue, std::string const & coordinate );

/* Récupère un tableau qui contient tout les nombres de la tilemap dans le fichier.
Récupération de la taille par référence. */
std::vector<int> read_tilemap( std::string const & fileContent, unsigned int & index,
    sf::Vector2u & tilemapSize, unsigned int & superiorLoopIndex );
