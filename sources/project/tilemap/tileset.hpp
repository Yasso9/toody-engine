#pragma once

#include <project/output-info/information.hpp>
#include <project/utility/utility.hpp>

class Tileset : public sf::RectangleShape
{
  public:
    Tileset();

    // Mettre public et mettre m_ devant
    bool isMouseOn { false };
    bool isPrint { false };

    /* Créer l'objet avec sa texture et sa police */
    void create( sf::Texture const & texture );

    void update( sf::Vector2f const mousePosition,
                 unsigned int & tile,
                 bool const buttonIsPress );

    /* Affiche ou n'affiche pas la Tileset en fonction de isPrint
    Affiche aussi le carré du curseur de la souris si elle
    passe par dessue le tileset */
    void render( sf::RenderWindow & target ) const;

    ~Tileset() = default;

  private:
    TileCursor m_tileCursor {};

    void update_tile_cursor( sf::Vector2f const cursorPosition );

    /* Verifie si la position fait partie de l'objet */
    bool include( sf::Vector2f const position ) const;
};