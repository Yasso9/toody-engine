#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#if defined( __GNUC__ )
    #pragma GCC diagnostic ignored "-Wold-style-cast"
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

#include "IMGUI/imgui-SFML.h"
#include "IMGUI/imgui.h"

#pragma GCC diagnostic pop

// P stands for pernnal fonctions (overloading ImGui)

namespace ImGui
{
    bool P_Begin( std::string const & name, bool * p_open = nullptr,
                  ImGuiWindowFlags const & flags = 0 );

    /**
     * @brief Check if a ImGui Window created with P_Begin is Focused
     * @return true if a ImGui Window created with P_Begin is Focused, false otherwise
     */
    bool P_IsAnyWindowFocused();

    void P_ResetWindowFocused();
} // namespace ImGui