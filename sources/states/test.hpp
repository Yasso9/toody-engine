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

    TestState( std::shared_ptr< sf::RenderWindow > window,
               Ressources const & ressources );
    virtual ~TestState() = default;

    void update() override;
    void render() override;
};