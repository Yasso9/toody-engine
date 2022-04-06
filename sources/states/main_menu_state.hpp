#pragma once

#include "states/state.hpp"

class MainMenuState final : public State
{
  public:
    MainMenuState( Ressources const & ressources );
    virtual ~MainMenuState() = default;

    void update() override;
    void render() const override;

  private:
    sf::RectangleShape m_background;
    sf::Text m_text;

    ButtonArray m_buttons;

    void init_background();
    void init_text();
    void init_buttons();
};