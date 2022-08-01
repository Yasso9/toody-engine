#include "imgui.hpp"

#include <functional> // for function
#include <stddef.h>   // for NULL

#include <SFML/Config.hpp> // for Uint8

#include "tools/assertion.hpp"       // for ASSERTION
#include "tools/global_variable.hpp" // for COLOR_RANGE

namespace ImGui
{
    void P_Begin( std::string name, std::function< void() > const & lambda )
    {
        P_Begin( name, NULL, lambda );
    }
    void P_Begin( std::string name, bool * isWindowOpen,
                  std::function< void() > const & lambda )
    {
        P_Begin( name, isWindowOpen, 0, lambda );
    }
    void P_Begin( std::string name, bool * isWindowOpen, ImGuiWindowFlags flags,
                  std::function< void() > const & lambda )
    {
        if ( isWindowOpen != NULL && ! *isWindowOpen )
        {
            return;
        }

        if ( ImGui::Begin( name.c_str(), isWindowOpen, flags ) )
        {
            lambda();
            ImGui::End();
        }
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