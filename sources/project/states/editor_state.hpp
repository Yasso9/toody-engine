#pragma once

#include <project/output-info/information.hpp>
#include <project/states/state.hpp>
#include <project/tilemap/tilemap.hpp>
#include <project/tilemap/tileset.hpp>

class EditorState final : public State
{
  public:
    EditorState( t_texturesMap const & textures, t_fontsMap const & fonts );

    // void copy_selection();
    // void paste_selection(sf::Vector2f const tablePosition);
    // void update_selection(sf::Vector2f beginPos, sf::Vector2f endPos);

    void update() override;
    void render( sf::RenderWindow & target ) override;

  private:
    enum Mode
    {
        Normal = 0,
        Selection
    };

    EditorMap m_tilemap;
    Tileset m_sheet {};

    // Widget
    ButtonArray m_buttons {};
    Mode m_mode {};
    Message m_debugInfo {};

    // View
    sf::View m_screenView {};
    sf::View m_mapView {}; // a mettre dans la classe tilemap

    // Selection
    sf::RectangleShape m_selectionRect {};
    std::vector<int> m_tableSelection {};

    /* Valeur de la case actuel dans la tilemap */
    unsigned int m_tile {};

    void init_map();
    void init_widget();
    void init_views();
    void init_selection_rect();

    void handle_keyboard_press( std::string const & inputName ) override;
    // void handle_mouse_button_press( std::string const & inputName ) override;
    // void handle_mouse_button_release( std::string const & inputName ) override;
    void handle_mouse_wheel_up() override;
    void handle_mouse_wheel_down() override;
    void handle_global_input() override;

    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_normal_mode();
    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_selection_mode();
    /* Fonction qui permet de séparer les actions de l'utilisateur */
    void update_colision_mode();
};
