#include "test.hpp"

#include <imgui/imgui.h>  // for ShowDemoWindow

TestState::TestState( GameContext & gameContext ) : State { gameContext } {}

void TestState::update_before( UpdateContext /* context */ )
{
    ImGui::ShowDemoWindow();
}

// void TestState::render() const {}