#pragma once

#include <functional>  // for function
#include <stdint.h>    // for uint32_t
#include <string>      // for string

#include <IMGUI/imgui.h>            // for ImGuiColorEditFlags, ImGuiInputTe...
#include <SFML/Graphics/Color.hpp>  // for Color

#include "maths/vector3.hpp"

// P stands for personnal fonctions (overloading ImGui)

namespace ImGui
{
    void P_Show ( std::string name, std::function< void() > const & lambda );
    void P_Show (
        std::string name, bool * isWindowOpen,
        std::function< void() > const & lambda );
    void P_Show (
        std::string name, bool * isWindowOpen, ImGuiWindowFlags flags,
        std::function< void() > const & lambda );

    /// @brief bindings for sfml colors
    bool P_ColorEditor (
        std::string const & label, sf::Color & color,
        ImGuiColorEditFlags flags = 0 );

    /// @brief unsigned int
    bool P_InputNumber (
        std::string const & label, unsigned int & value, int step = 1,
        int step_fast = 100, ImGuiInputTextFlags flags = 0 );

    bool P_InputVector3F (
        std::string const & label, math::Vector3F & vector,
        std::function< void( math::Vector3F & ) > const & resultFonctor =
            [] ( math::Vector3F & /* vectorResult */ ) {},
        char const * format = "%.3f", ImGuiInputTextFlags flags = 0 );

    namespace color
    {
        uint32_t  to_integer ( sf::Color color );
        void      to_table ( sf::Color sfmlColor, float tableColor[4] );
        sf::Color table_to_sfml ( float tableColor[4] );
    };  // namespace color

}  // namespace ImGui