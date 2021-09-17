#pragma once

#include <project/state/state.hpp>

class MainMenuState final : public State
{
public:
    MainMenuState(
        std::map< TextureKey, sf::Texture > const & textures,
        std::map< FontKey, sf::Font > const & fonts,
        sf::Vector2u const & windowSize
    );

    void update() override;
    void render( sf::RenderWindow & target ) override;

private:
    sf::RectangleShape m_background {};
    sf::Text m_text {};

    ButtonVector m_buttons {};

    void init_background();
    void init_text();
    void init_buttons();
};