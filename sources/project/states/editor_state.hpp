#pragma once

#include <project/output-info/information.hpp>
#include <project/states/state.hpp>
#include <project/tilemap/tilemap.hpp>
#include <project/tilemap/tileset.hpp>

class EditorState final : public State
{
  public:
    EditorState( Ressources const & ressources, Settings const & settings );

    void update() override;
    void render( sf::RenderWindow & target ) override;

  private:
    enum Mode
    {
        Normal = 0,
        Selection,
        Colision
    };

    EditorMap m_tilemap;
    Tileset m_tileset;

    // Widget
    ButtonArray m_buttons;
    Message m_debugInformations;
    Mode m_mode {};

    sf::View m_view {}; // a mettre dans la classe tilemap

    // Selection
    // sf::RectangleShape m_selectionRect {};
    // std::vector<int> m_tableSelection {};

    /* Number of the tile selected */
    unsigned int m_tile {};

    void init_map();
    void init_selection_rect();

    void handle_keyboard_press( std::string const & inputName ) override;
    // void handle_mouse_button_press( std::string const & inputName ) override;
    // void handle_mouse_button_release( std::string const & inputName ) override;
    void handle_mouse_wheel_up() override;
    void handle_mouse_wheel_down() override;
    void handle_current_input() override;

    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_normal_mode();
    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_selection_mode();
    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_colision_mode();
};
