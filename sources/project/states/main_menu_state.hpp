#pragma once

#include <project/states/state.hpp>

class MainMenuState final : public State
{
  public:
    MainMenuState( Ressources const & ressources, Settings const & settings );

    void update() override;
    void render( sf::RenderWindow & target ) override;

  private:
    sf::RectangleShape m_background {};
    sf::Text m_text {};

    ButtonArray m_buttons;

    void init_background();
    void init_text();
    void init_buttons();
};