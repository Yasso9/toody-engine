#pragma once

#include <string>             // for string

#include <fmt/format.h>       // for format
#include <imgui/imgui.h>      // for ImGuiInputTextFlags, ImGuiSliderFlags, etc

#include "maths/vector2.hpp"  // for Vector2F, Vector2I, Vector2U

class SubWindow;

namespace ImGui
{
    // ####################################################################
    // ############################# WINDOW ###############################
    // ####################################################################

    constexpr ImGuiWindowFlags DEFAULT_WINDOW_FLAGS =
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
    constexpr const char * DEFAULT_FLOAT_FORMAT = "%.3f";

    bool BeginWindow ( SubWindow & subWindow );
    bool BeginWindow ( std::string const & name,
                       ImGuiWindowFlags    flags = DEFAULT_WINDOW_FLAGS );
    bool BeginWindow ( std::string const & name, bool & isWindowOpen,
                       ImGuiWindowFlags flags = DEFAULT_WINDOW_FLAGS );

    void EndWindow ();

    // TODO make Begin and End deprecated
    // [[deprecated(
    //     "Please use bool Begin ( std::string const & name) "
    //     "instead" )]] IMGUI_API bool
    //     Begin ( char const * name, bool * p_open = NULL,
    //             ImGuiWindowFlags flags = 0 );

    // ####################################################################
    // ############################# INPUT ################################
    // ####################################################################

    bool InputVec ( std::string const & label, math::Vector2F & vector,
                    std::string const & format = DEFAULT_FLOAT_FORMAT,
                    ImGuiInputTextFlags flags  = 0 );
    bool InputVec ( std::string const & label, math::Vector2I & vec,
                    ImGuiInputTextFlags flags = 0 );

    // ####################################################################
    // ############################ SLIDERS ###############################
    // ####################################################################

    bool SliderVec ( std::string const & label, math::Vector2F & vec, float min,
                     float               max,
                     std::string const & format = DEFAULT_FLOAT_FORMAT,
                     ImGuiSliderFlags    flags  = 0 );
    bool SliderVec ( std::string const & label, math::Vector2I & vec, int min,
                     int max, ImGuiSliderFlags flags = 0 );
    bool SliderVec ( std::string const & label, math::Vector2U & vec,
                     unsigned int min, unsigned int max,
                     ImGuiSliderFlags flags = 0 );

    // ####################################################################
    // ######################## WINDOWS SCROLLING #########################
    // ####################################################################

    math::Vector2F GetScroll ();
    void           SetScroll ( math::Vector2F const & scroll );
    math::Vector2F GetScrollMax ();

    // ####################################################################
    // ############################### TEXT ###############################
    // ####################################################################

    template< typename... Args >
    void TextFmt ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        std::string formattedString =
            fmt::format( fmt, std::forward< Args >( args )... );
        ImGui::Text( "%s", formattedString.c_str() );
    }

    // ####################################################################
    // ############################### STYLE ##############################
    // ####################################################################

    void ResetStyle ();
}  // namespace ImGui
