#include "test.hpp"

#include <IMGUI/imgui.h>  // for ShowDemoWindow

TestState::TestState() : State( State::E_List::Test ) {}

void TestState::update_before( UpdateContext /* context */ )
{
    ImGui::ShowDemoWindow();
}

// void TestState::render() const {}