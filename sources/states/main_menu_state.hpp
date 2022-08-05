#pragma once

#include <SFML/Graphics/RectangleShape.hpp>  // for RectangleShape
#include <SFML/Graphics/Text.hpp>            // for Text

#include "input/button_array.hpp"  // for ButtonArray
#include "states/state.hpp"        // for State

class Render;

class MainMenuState final : public State
{
  public:
    MainMenuState();
    virtual ~MainMenuState() = default;

    void update_before ( float deltaTime ) override;
    void render_before ( Render & render ) const override;

  private:
    sf::RectangleShape m_background;
    sf::Text           m_text;

    ButtonArray m_buttons;

    void init_background ();
    void init_text ();
    void init_buttons ();
};