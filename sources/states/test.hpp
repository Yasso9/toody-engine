#pragma once

#include <iostream>

#include "states/state.hpp"

#include "graphics3D/openGL.hpp"
#include "tools/tools.hpp"

class TestState final : public State
{
  public:
    TestState();
    virtual ~TestState() = default;

    void update_extra( float deltaTime ) override;
    // void render( sf::RenderTarget & target ) const override;
};