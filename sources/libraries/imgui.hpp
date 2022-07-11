#pragma once

#include <IMGUI/imgui-SFML.h>
#include <IMGUI/imgui.h>

// P stands for personnal fonctions (overloading ImGui)

namespace ImGui
{
    bool P_Begin( std::string const & name, bool * p_open = nullptr,
                  ImGuiWindowFlags const & flags = 0 );

    /**
     * @brief Check if a ImGui Window created with P_Begin is Focused
     * @return true if a ImGui Window created with P_Begin is Focused, false otherwise
     */
    bool P_IsAnyWindowFocused();
    /**
     * @brief Check if a ImGui Window created with P_Begin is Hovered
     * @return true if a ImGui Window created with P_Begin is Hovered, false otherwise
     */
    bool P_IsAnyWindowHovered();

    void P_ResetVariables();
} // namespace ImGui