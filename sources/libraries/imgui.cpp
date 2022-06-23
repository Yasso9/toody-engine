#include "imgui.hpp"

namespace ImGui
{
    static bool g_isAnyWindowFocused { false };

    bool P_Begin( std::string const & name, bool * p_open,
                  ImGuiWindowFlags const & flags )
    {
        bool isWindowBegin { ImGui::Begin( name.c_str(), p_open, flags ) };

        if ( isWindowBegin )
        {
            g_isAnyWindowFocused =
                g_isAnyWindowFocused || ImGui::IsWindowFocused();
        }

        return isWindowBegin;
    }

    bool P_IsAnyWindowFocused()
    {
        return g_isAnyWindowFocused;
    }

    void P_ResetWindowFocused()
    {
        g_isAnyWindowFocused = false;
    }
} // namespace ImGui