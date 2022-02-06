#pragma once

#include "states/state.hpp"

#include "output_info/information.hpp"
#include "tilemap/tilemap.hpp"
#include "tilemap/tileset.hpp"

class EditorState final : public State
{
  public:
    EditorState( std::shared_ptr<sf::RenderWindow> window,
                 Ressources const & ressources, Settings const & settings );

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

    sf::View m_view; // a mettre dans la classe tilemap

    // Selection
    // sf::RectangleShape m_selectionRect {};
    // std::vector<int> m_tableSelection {};

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
