#pragma once

#include <iostream>

#include "states/state.hpp"

#include "graphics/openGL.hpp"
#include "tools/tools.hpp"

class TestState final : public State
{
  public:
    sf::Texture texture;
    unsigned int VAO, VBO;
    sf::Shader ourShader;

    TestState();
    virtual ~TestState() = default;

    void update() override;
    void render() const override;
};