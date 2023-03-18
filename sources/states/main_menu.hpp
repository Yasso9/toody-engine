#pragma once

#include <SFML/Graphics/RectangleShape.hpp>  // for RectangleShape
#include <SFML/Graphics/Text.hpp>            // for Text

#include "graphics2D/button_array.hpp"  // for ButtonArray
#include "states/state.hpp"             // for State

class Render;

class MainMenuState final : public State
{
    std::vector< sf::Text > m_texts;
    sf::RectangleShape      m_background;
    sf::RectangleShape      m_menuBackground;

  public:
    MainMenuState( GameContext & gameContext );
    virtual ~MainMenuState() = default;

    void update ( UpdateContext context ) override;
    void render ( RenderContext context ) const override;
};