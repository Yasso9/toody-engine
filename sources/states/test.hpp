#pragma once

#include <iostream>

#include "states/state.hpp"

#include "graphics/openGL.hpp"
#include "tools/tools.hpp"

class TestState final : public State
{
  public:
    TestState();
    virtual ~TestState() = default;

    void update() override;
    void render() const override;
};