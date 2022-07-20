#pragma once

#include "states/state.hpp"

class MainMenuState final : public State
{
  public:
    MainMenuState();
    virtual ~MainMenuState() = default;

    void update_extra( float deltaTime ) override;
    // void render( sf::RenderTarget & target ) const override;

  private:
    sf::RectangleShape m_background;
    sf::Text m_text;

    ButtonArray m_buttons;

    void init_background();
    void init_text();
    void init_buttons();

    /// @todo delete that function
    void extra_events();
};