#pragma once

#include "states/state.hpp"

#include "output_info/information.hpp"
#include "tilemap/tilemap.hpp"
#include "tilemap/tileset.hpp"

class EditorState final : public State
{
  public:
    EditorState( std::shared_ptr<sf::RenderWindow> window,
                 Ressources const & ressources );

    void update() override;
    void render() override;

  private:
    enum Type
    {
        Normal = 0,
        Selection,
        Colision
    };

    EditorMap m_tilemap;
    Tileset m_tileset;

    ButtonArray m_buttons;
    Type m_type;

    sf::View m_view; // à mettre dans la classe tilemap

    // Selection
    // sf::RectangleShape m_selectionRect {};
    // std::vector<int> m_tableSelection {};

    void keyboard_pressed( sf::Event event ) override;
    void mouse_scroll( float const & deltaScroll ) override;

    void init_map();
    void init_selection_rect();

    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_normal_mode();
    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_selection_mode();
    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_colision_mode();
};
