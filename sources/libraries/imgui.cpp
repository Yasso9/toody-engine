#include "imgui.hpp"

#include <functional>  // for function
#include <stddef.h>    // for NULL

#include <SFML/Config.hpp>  // for Uint8

#include "graphics2D/constants.hpp"  // for COLOR_RANGE
#include "tools/assertion.hpp"       // for ASSERTION

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
        if ( ! subWindow.showWindow )
        {
            return false;
        }
        return ImGui::Begin( subWindow.windowName.c_str(),
                             &subWindow.showWindow );
    }

    bool Begin ( std::string const & name, ImGuiWindowFlags flags )
    {
        return ImGui::Begin( name.c_str(), NULL, flags );
    }

    bool Begin ( std::string const & name, bool & isWindowOpen,
                 ImGuiWindowFlags flags )
    {
        return ImGui::Begin( name.c_str(), &isWindowOpen, flags );
    }
}  // namespace ImGui
