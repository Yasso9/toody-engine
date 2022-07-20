#include "test.hpp"

#include "libraries/imgui.hpp"

TestState::TestState() : State( State::E_List::Test ) {}

void TestState::update_extra( float /* deltaTime */ )
{
    ImGui::ShowDemoWindow();
}

// void TestState::render() const {}