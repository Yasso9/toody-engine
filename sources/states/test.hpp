#pragma once

#include "states/state.hpp"  // for State

class TestState final : public State
{
  public:
    TestState( GameContext & gameContext );
    virtual ~TestState() = default;

    void update_before ( UpdateContext context ) override;
    // void render( sf::RenderTarget & target ) const override;
};