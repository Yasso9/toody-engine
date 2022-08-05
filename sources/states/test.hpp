#pragma once

#include "states/state.hpp"  // for State

class TestState final : public State
{
  public:
    TestState();
    virtual ~TestState() = default;

    void update_before ( float deltaTime ) override;
    // void render( sf::RenderTarget & target ) const override;
};