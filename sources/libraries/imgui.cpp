#include "imgui.hpp"

#include "tools/assertion.hpp"
#include "tools/global_variable.hpp"

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

    bool P_ColorEditor( std::string const & label, sf::Color & color,
                        ImGuiColorEditFlags flags )
    {
        float colorTable[4] {};
        color::to_table( color, colorTable );

        bool result = ImGui::ColorEdit4( label.c_str(), colorTable, flags );

        color = color::table_to_sfml( colorTable );
        return result;
    }

    bool P_InputNumber( std::string const & label, unsigned int & value,
                        int step, int step_fast, ImGuiInputTextFlags flags )
    {
        return InputScalar(
            label.c_str(),
            ImGuiDataType_U32,
            reinterpret_cast< void * >( &value ),
            reinterpret_cast< void * >( step > 0 ? &step : NULL ),
            reinterpret_cast< void * >( step_fast > 0 ? &step_fast : NULL ),
            "%u",
            flags );
    }
    bool P_InputNumber( std::string const & label, std::size_t & value,
                        int step, int step_fast, ImGuiInputTextFlags flags )
    {
        return InputScalar(
            label.c_str(),
            ImGuiDataType_U32,
            reinterpret_cast< void * >( &value ),
            reinterpret_cast< void * >( step > 0 ? &step : NULL ),
            reinterpret_cast< void * >( step_fast > 0 ? &step_fast : NULL ),
            "%zu",
            flags );
    }

    namespace color
    {
        uint32_t to_integer( sf::Color color )
        {
            return ( static_cast< ImU32 >( color.a ) << 24 )
                   | ( static_cast< ImU32 >( color.b ) << 16 )
                   | ( static_cast< ImU32 >( color.g ) << 8 )
                   | ( static_cast< ImU32 >( color.r ) << 0 );
        }

        void to_table( sf::Color sfmlColor, float tableColor[4] )
        {
            ASSERTION( ( sfmlColor.r >= 0 && sfmlColor.r <= 255 )
                           && ( sfmlColor.g >= 0 && sfmlColor.g <= 255 )
                           && ( sfmlColor.b >= 0 && sfmlColor.b <= 255 )
                           && ( sfmlColor.a >= 0 && sfmlColor.a <= 255 ),
                       "SFML colors value must be between 0 and 255" );

            tableColor[0] = static_cast< float >( sfmlColor.r ) / COLOR_RANGE;
            tableColor[1] = static_cast< float >( sfmlColor.g ) / COLOR_RANGE;
            tableColor[2] = static_cast< float >( sfmlColor.b ) / COLOR_RANGE;
            tableColor[3] = static_cast< float >( sfmlColor.a ) / COLOR_RANGE;

            ASSERTION( ( tableColor[0] >= 0.f && tableColor[0] <= 1.f )
                           && ( tableColor[1] >= 0.f && tableColor[1] <= 1.f )
                           && ( tableColor[2] >= 0.f && tableColor[2] <= 1.f )
                           && ( tableColor[3] >= 0.f && tableColor[3] <= 1.f ),
                       "Table colors value must be between 0 and 1" );
        }

        sf::Color table_to_sfml( float tableColor[4] )
        {
            ASSERTION( ( tableColor[0] >= 0.f && tableColor[0] <= 1.f )
                           && ( tableColor[1] >= 0.f && tableColor[1] <= 1.f )
                           && ( tableColor[2] >= 0.f && tableColor[2] <= 1.f )
                           && ( tableColor[3] >= 0.f && tableColor[3] <= 1.f ),
                       "Table colors value must be between 0 and 1" );

            return { static_cast< sf::Uint8 >( tableColor[0] * COLOR_RANGE ),
                     static_cast< sf::Uint8 >( tableColor[1] * COLOR_RANGE ),
                     static_cast< sf::Uint8 >( tableColor[2] * COLOR_RANGE ),
                     static_cast< sf::Uint8 >( tableColor[3] * COLOR_RANGE ) };
        }
    }; // namespace color
} // namespace ImGui