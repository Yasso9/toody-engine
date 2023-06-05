#pragma once

#include "states/state.hpp"  // for State

class TestState final : public State
{
  public:
    TestState();

    void update ( UpdateContext & context ) override;
};
