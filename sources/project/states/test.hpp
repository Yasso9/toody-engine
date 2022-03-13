#pragma once

#include <iostream>

#include "states/state.hpp"

#include "graphics/openGL.hpp"
#include "tools/tools.hpp"

class TestState final : public State
{
  public:
    sf::Texture texture;
    unsigned int VAO;
    sf::Shader ourShader;

    TestState( std::shared_ptr<sf::RenderWindow> window,
               Ressources const & ressources, Settings const & settings );
    virtual ~TestState() = default;

    void update( float const & /* deltaTime */ ) override;
    void render() override;
};