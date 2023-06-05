#include "test.hpp"

#include <imgui/imgui.h>  // for ShowDemoWindow

TestState::TestState() : State {} {}

void TestState::update( UpdateContext & /* context */ )
{
    ImGui::ShowDemoWindow();
}
