#pragma once

#include "states/state.hpp"

class MainMenuState final : public State
{
  public:
    MainMenuState( std::shared_ptr<sf::RenderWindow> window,
                   Ressources const & ressources, Settings const & settings );
    virtual ~MainMenuState() = default;

    void update( float const & deltaTime ) override;
    void render() override;

  private:
    sf::RectangleShape m_background;
    sf::Text m_text;
    ButtonArray m_buttons;

    T_KeyboardInputMap init_keyboard_action() const override;
    T_MouseInputMap init_mouse_action() const override;

    void init_background();
    void init_text();
    void init_buttons();
};