#include "test.hpp"

#include "tools/imgui.hpp"

TestState::TestState() : State( State::E_List::Test ) {}

void TestState::update()
{
    ImGui::ShowDemoWindow();
}

void TestState::render() const {}