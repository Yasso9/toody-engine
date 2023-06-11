#include "imgui.hpp"

#include <functional>  // for function
#include <stddef.h>    // for NULL

#include <SFML/Config.hpp>     // for Uint8
#include <imgui/imgui-SFML.h>  // for ImGui::SFML::UpdateFontTexture

#include "application/configuration.hpp"  // for Settings
#include "application/resources.hpp"      // for get_font
#include "graphics2D/constants.hpp"       // for COLOR_RANGE
#include "imgui/sub_window.hpp"           // for SubWindow
#include "tools/assertion.hpp"            // for ASSERTION
#include "tools/traces.hpp"

namespace ImGui
{
    // ####################################################################
    // ############################# WINDOW ###############################
    // ####################################################################

    namespace
    {
        void WindowConfig ()
        {
            ImGui::SetNextWindowBgAlpha( 0.5f );
        }
    }  // namespace

    bool BeginWindow ( SubWindow & subWindow )
    {
        if ( ! subWindow.is_window_enabled() )
        {
            return false;
        }
        return ImGui::BeginWindow( subWindow.m_windowName, subWindow.m_show,
                                   subWindow.m_flags );
    }

    bool BeginWindow ( std::string const & name, ImGuiWindowFlags flags )
    {
        WindowConfig();
        return ImGui::Begin( name.c_str(), NULL, flags );
    }

    bool BeginWindow ( std::string const & name, bool & isWindowOpen,
                       ImGuiWindowFlags flags )
    {
        if ( ! isWindowOpen )
        {
            return false;
        }
        WindowConfig();
        return ImGui::Begin( name.c_str(), &isWindowOpen, flags );
    }

    void EndWindow ()
    {
        ImGui::End();
    }

    // ####################################################################
    // ############################# INPUT ################################
    // ####################################################################

    bool InputVec ( std::string const & label, math::Vector2F & vec,
                    std::string const & format, ImGuiInputTextFlags flags )
    {
        float vecArray[2] = { vec.x, vec.y };
        if ( ImGui::InputFloat2( label.c_str(), vecArray, format.c_str(),
                                 flags ) )
        {
            vec.x = vecArray[0];
            vec.y = vecArray[1];
            return true;
        }
        return false;
    }

    bool InputVec ( std::string const & label, math::Vector2I & vec,
                    ImGuiInputTextFlags flags )
    {
        int vecArray[2] = { vec.x, vec.y };
        if ( ImGui::InputInt2( label.c_str(), vecArray, flags ) )
        {
            vec.x = static_cast< int >( vecArray[0] );
            vec.y = static_cast< int >( vecArray[1] );
            return true;
        }
        return false;
    }

    // ####################################################################
    // ############################ SLIDERS ###############################
    // ####################################################################

    bool SliderVec ( std::string const & label, math::Vector2F & vec, float min,
                     float max, std::string const & format,
                     ImGuiSliderFlags flags )
    {
        float vecArray[2] = { vec.x, vec.y };
        if ( ImGui::SliderFloat2( label.c_str(), vecArray, min, max,
                                  format.c_str(), flags ) )
        {
            vec.x = vecArray[0];
            vec.y = vecArray[1];
            return true;
        }
        return false;
    }

    bool SliderVec ( std::string const & label, math::Vector2I & vec, int min,
                     int max, ImGuiSliderFlags flags )
    {
        int vecArray[2] = { vec.x, vec.y };
        if ( ImGui::SliderInt2( label.c_str(), vecArray, min, max, "%d",
                                flags ) )
        {
            vec.x = vecArray[0];
            vec.y = vecArray[1];
            return true;
        }
        return false;
    }

    bool SliderVec ( std::string const & label, math::Vector2U & vec,
                     unsigned int min, unsigned int max,
                     ImGuiSliderFlags flags )
    {
        int vecArray[2] = { static_cast< int >( vec.x ),
                            static_cast< int >( vec.y ) };
        if ( ImGui::SliderInt2( label.c_str(), vecArray,
                                static_cast< int >( min ),
                                static_cast< int >( max ), "%u", flags ) )
        {
            vec.x = static_cast< unsigned int >( vecArray[0] );
            vec.y = static_cast< unsigned int >( vecArray[1] );
            return true;
        }
        return false;
    }

    // ####################################################################
    // ######################## WINDOWS SCROLLING #########################
    // ####################################################################

    math::Vector2F GetScroll ()
    {
        return math::Vector2F { ImGui::GetScrollX(), ImGui::GetScrollY() };
    }

    void SetScroll ( math::Vector2F const & scroll )
    {
        ImGui::SetScrollX( scroll.x );
        ImGui::SetScrollY( scroll.y );
    }

    math::Vector2F GetScrollMax ()
    {
        return math::Vector2F { ImGui::GetScrollMaxX(),
                                ImGui::GetScrollMaxY() };
    }

    // ####################################################################
    // ######################## WINDOWS UTILITIES #########################
    // ####################################################################

    bool IsWindowFocusedOrHovered ( ImGuiFocusedFlags flags )
    {
        return ImGui::IsWindowFocused( flags )
               || ImGui::IsWindowHovered( flags );
    }

    // ####################################################################
    // ############################### STYLE ##############################
    // ####################################################################

    void ResetStyle ()
    {
        // float uiScale = std::sqrt( this->get_display_scale().x
        //    * this->get_display_scale().y );
        float uiScale   = Config::get_instance().get_ui_scale();
        float fontScale = Config::get_instance().get_font_scale();
        Trace::Info( "UI scale: {}", uiScale );
        Trace::Info( "Font scale: {}", fontScale );

        ImGuiIO & io = ImGui::GetIO();
        io.Fonts->Clear();
        // io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromFileTTF(
            resource::font::FIRA_MONO.string().c_str(), fontScale * 16.f );
        if ( ! ImGui::SFML::UpdateFontTexture() )
        {
            Trace::Error( "Failed to setup font texture!" );
        }

        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGuiStyle & style = ImGui::GetStyle();

        style.WindowPadding     = ImVec2 { 10.f, 10.f };
        style.FramePadding      = ImVec2 { 10.f, 10.f };
        style.CellPadding       = ImVec2 { 10.f, 10.f };
        style.ItemSpacing       = ImVec2 { 10.f, 10.f };
        style.ItemInnerSpacing  = ImVec2 { 10.f, 10.f };
        style.TouchExtraPadding = ImVec2 { 0.f, 0.f };

        style.IndentSpacing = 15.f;
        style.ScrollbarSize = 15.f;
        style.GrabMinSize   = 15.f;

        style.WindowBorderSize = 0.f;
        style.ChildBorderSize  = 0.f;
        style.FrameBorderSize  = 0.f;
        style.PopupBorderSize  = 0.f;
        style.TabBorderSize    = 0.f;

        style.WindowRounding    = 6.f;
        style.ChildRounding     = 6.f;
        style.FrameRounding     = 6.f;
        style.PopupRounding     = 6.f;
        style.ScrollbarRounding = 6.f;
        style.GrabRounding      = 6.f;
        style.TabRounding       = 6.f;

        style.WindowTitleAlign         = ImVec2 { 0.5f, 0.5f };
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.ColorButtonPosition      = ImGuiDir_Right;
        style.ButtonTextAlign          = ImVec2 { 0.5f, 0.5f };
        style.SelectableTextAlign      = ImVec2 { 0.f, 0.5f };
        // style.SeparatorTextBorderSize  = 3.f;
        // style.SeparatorTextAlign       = ImVec2 { 0.5f, 0.5f };
        // style.SeparatorTextPadding     = ImVec2 { 0.5f, 0.5f };
        style.LogSliderDeadzone = 0.5f;

        style.DisplaySafeAreaPadding = ImVec2 { 3.f, 3.f };

        style.Colors[ImGuiCol_TableHeaderBg]     = ImVec4 { 0.25f, 0.25f, 0.25f,
                                                        1.00f };
        style.Colors[ImGuiCol_TableBorderLight]  = ImVec4 { 0.80f, 0.80f, 0.80f,
                                                           1.00f };
        style.Colors[ImGuiCol_TableBorderStrong] = ImVec4 { 0.70f, 0.70f, 0.70f,
                                                            1.00f };
        style.Colors[ImGuiCol_Text] = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4 { 0.0f, 0.0f, 0.0f,
                                                         1.0f };

        style.ScaleAllSizes( uiScale );
    }
}  // namespace ImGui
