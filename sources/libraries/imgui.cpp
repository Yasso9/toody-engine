#include "imgui.hpp"

namespace ImGui
{
    /// @todo améliorer ces infos pour pouvoir les récupéré à n'importe quelle moment de la boucle
    static bool g_isAnyWindowFocused { false };
    static bool g_isAnyWindowHovered { false };

    bool P_Begin( std::string const & name, bool * p_open,
                  ImGuiWindowFlags const & flags )
    {
        bool isWindowBegin { ImGui::Begin( name.c_str(), p_open, flags ) };

        if ( isWindowBegin )
        {
            g_isAnyWindowFocused =
                g_isAnyWindowFocused || ImGui::IsWindowFocused();
            g_isAnyWindowHovered =
                g_isAnyWindowHovered || ImGui::IsWindowHovered();
        }

        return isWindowBegin;
    }

    bool P_IsAnyWindowFocused()
    {
        return g_isAnyWindowFocused;
    }

    bool P_IsAnyWindowHovered()
    {
        return g_isAnyWindowHovered;
    }

    void P_ResetVariables()
    {
        g_isAnyWindowFocused = false;
        g_isAnyWindowHovered = false;
    }
} // namespace ImGui