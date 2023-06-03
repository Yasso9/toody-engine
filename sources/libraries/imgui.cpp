#include "imgui.hpp"

#include <functional>  // for function
#include <stddef.h>    // for NULL

#include <SFML/Config.hpp>  // for Uint8

#include "game/resources.hpp"        // for get_font
#include "game/settings.hpp"         // for Settings
#include "graphics2D/constants.hpp"  // for COLOR_RANGE
#include "libraries/sub_window.hpp"  // for SubWindow
#include "tools/assertion.hpp"       // for ASSERTION
#include "tools/traces.hpp"

namespace ImGui
{
    void P_Show ( std::string name, std::function< void() > const & lambda )
    {
        P_Show( name, NULL, lambda );
    }

    void P_Show ( std::string name, bool * isWindowOpen,
                  std::function< void() > const & lambda )
    {
        P_Show( name, isWindowOpen, 0, lambda );
    }

    void P_Show ( std::string name, bool * isWindowOpen, ImGuiWindowFlags flags,
                  std::function< void() > const & lambda )
    {
        if ( isWindowOpen != NULL && ! *isWindowOpen )
        {
            return;
        }

        if ( ImGui::Begin( name.c_str(), isWindowOpen, flags ) )
        {
            lambda();
        }
        ImGui::End();
    }

    bool P_ColorEditor ( std::string const & label, sf::Color & color,
                         ImGuiColorEditFlags flags )
    {
        float colorTable[4] {};
        color::to_table( color, colorTable );

        bool result = ImGui::ColorEdit4( label.c_str(), colorTable, flags );

        color = color::table_to_sfml( colorTable );
        return result;
    }

    bool P_InputNumber ( std::string const & label, unsigned int & value,
                         int step, int step_fast, ImGuiInputTextFlags flags )
    {
        return InputScalar(
            label.c_str(), ImGuiDataType_U32,
            reinterpret_cast< void * >( &value ),
            reinterpret_cast< void * >( step > 0 ? &step : NULL ),
            reinterpret_cast< void * >( step_fast > 0 ? &step_fast : NULL ),
            "%u", flags );
    }

    bool P_InputVector3F (
        std::string const & label, math::Vector3F & vector,
        std::function< void( math::Vector3F & ) > const & resultFonctor,
        char const * format, ImGuiInputTextFlags flags )
    {
        float vectorTable[3] { vector.x, vector.y, vector.z };

        bool returnValue = InputFloat3( label.c_str(), vectorTable, format,
                                        flags );

        vector.x = vectorTable[0];
        vector.y = vectorTable[1];
        vector.z = vectorTable[2];

        resultFonctor( vector );

        return returnValue;
    }

    namespace color
    {
        uint32_t to_integer ( sf::Color color )
        {
            return ( static_cast< ImU32 >( color.a ) << 24 )
                   | ( static_cast< ImU32 >( color.b ) << 16 )
                   | ( static_cast< ImU32 >( color.g ) << 8 )
                   | ( static_cast< ImU32 >( color.r ) << 0 );
        }

        void to_table ( sf::Color sfmlColor, float tableColor[4] )
        {
            ASSERTION( ( sfmlColor.r >= 0 && sfmlColor.r <= 255 )
                           && ( sfmlColor.g >= 0 && sfmlColor.g <= 255 )
                           && ( sfmlColor.b >= 0 && sfmlColor.b <= 255 )
                           && ( sfmlColor.a >= 0 && sfmlColor.a <= 255 ),
                       "SFML colors value must be between 0 and 255" );

            tableColor[0] = static_cast< float >( sfmlColor.r ) / COLOR_RANGE_U;
            tableColor[1] = static_cast< float >( sfmlColor.g ) / COLOR_RANGE_U;
            tableColor[2] = static_cast< float >( sfmlColor.b ) / COLOR_RANGE_U;
            tableColor[3] = static_cast< float >( sfmlColor.a ) / COLOR_RANGE_U;

            ASSERTION( ( tableColor[0] >= 0.f && tableColor[0] <= 1.f )
                           && ( tableColor[1] >= 0.f && tableColor[1] <= 1.f )
                           && ( tableColor[2] >= 0.f && tableColor[2] <= 1.f )
                           && ( tableColor[3] >= 0.f && tableColor[3] <= 1.f ),
                       "Table colors value must be between 0 and 1" );
        }

        sf::Color table_to_sfml ( float tableColor[4] )
        {
            ASSERTION( ( tableColor[0] >= 0.f && tableColor[0] <= 1.f )
                           && ( tableColor[1] >= 0.f && tableColor[1] <= 1.f )
                           && ( tableColor[2] >= 0.f && tableColor[2] <= 1.f )
                           && ( tableColor[3] >= 0.f && tableColor[3] <= 1.f ),
                       "Table colors value must be between 0 and 1" );

            return { static_cast< sf::Uint8 >( tableColor[0] * COLOR_RANGE_U ),
                     static_cast< sf::Uint8 >( tableColor[1] * COLOR_RANGE_U ),
                     static_cast< sf::Uint8 >( tableColor[2] * COLOR_RANGE_U ),
                     static_cast< sf::Uint8 >( tableColor[3]
                                               * COLOR_RANGE_U ) };
        }
    };  // namespace color
}  // namespace ImGui

namespace ImGui_P
{
    void Text ( std::string string )
    {
        ImGui::Text( "%s", string.c_str() );
    }

    void Text ( std::ostringstream const & stream )
    {
        ImGui_P::Text( stream.str() );
    }
}  // namespace ImGui_P

// TODO all the function above will be deprecated : remove or improve them
namespace ImGui
{
    bool BeginWindow ( SubWindow & subWindow )
    {
        if ( ! subWindow.is_enabled() )
        {
            return false;
        }
        return ImGui::Begin( subWindow.m_windowName, subWindow.m_show,
                             subWindow.m_flags );
    }

    bool Begin ( std::string const & name, ImGuiWindowFlags flags )
    {
        return ImGui::Begin( name.c_str(), NULL, flags );
    }

    bool Begin ( std::string const & name, bool & isWindowOpen,
                 ImGuiWindowFlags flags )
    {
        if ( ! isWindowOpen )
        {
            return false;
        }
        return ImGui::Begin( name.c_str(), &isWindowOpen, flags );
    }

    void ResetStyle ()
    {
        // float uiScale = std::sqrt( this->get_display_scale().x
        //    * this->get_display_scale().y );
        float uiScale   = Settings::get_instance().get_ui_scale();
        float fontScale = Settings::get_instance().get_font_scale();
        Trace::Info( "UI scale: {}", uiScale );
        Trace::Info( "Font scale: {}", fontScale );

        ImGuiIO & io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF( resources::fonts::INTER_REGULAR.c_str(),
                                      fontScale * 20.f );

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
