#include "tile_selector.hpp"

#include <sstream>  // for operator<<, basic_ostream, stri...
#include <string>   // for char_traits, allocator, basic_s...

#include <IMGUI/imgui-SFML.h>       // for Image
#include <IMGUI/imgui.h>            // for GetWindowDrawList, IsWindowHovered
#include <SFML/Graphics/Color.hpp>  // for Color
#include <SFML/Window/Mouse.hpp>    // for Mouse, Mouse::Button, Mouse::Left

#include "input/input.hpp"            // for get_mouse_position, is_pressed
#include "libraries/imgui.hpp"        // for table_to_sfml, to_integer, to_t...
#include "main/resources.hpp"         // for get_texture
#include "maths/geometry/point.tpp"   // for Point::Point<Type>
#include "maths/numerics.hpp"         // for division_reminder_u
#include "maths/vector2.hpp"          // for Vector2F, Vector2, Vector2I
#include "maths/vector2.tpp"          // for operator<<, operator+, Vector2:...
#include "tools/global_variable.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZE_U

namespace tile
{
    Selector::Selector()
      : m_tileset { resources::get_texture( "ground.png" ) },
        m_tileSelected { std::nullopt },
        m_isGridEnabled { true },
        m_gridColor { Color::RGBA { 118, 118, 118, 255 } }
    {}

    Tileset const & Selector::get_tileset() const
    {
        return m_tileset;
    }

    std::optional< unsigned int > Selector::get_tile_selected() const
    {
        return m_tileSelected;
    }

    void Selector::update_before( float /* deltaTime */ )
    {
        if ( ImGui::Begin( "Tile Selector" ) )
        {
            ImGui::Checkbox( "Enable grid", &m_isGridEnabled );
            ImGui::ColorEdit4( "Color Edit", m_gridColor.to_table() );

            m_tileset.set_position( ImGui::GetCursorScreenPos() );
            ImGui::Image( m_tileset.get_texture() );

            if ( m_isGridEnabled )
            {
                this->update_grid( *ImGui::GetWindowDrawList() );
            }
            this->update_selection( *ImGui::GetWindowDrawList() );
        }
        ImGui::End();
    }

    void Selector::update_grid( ImDrawList & drawList )
    {
        math::Vector2F const scrolling { 0.f, 0.f };

        // Tilemap Border
        drawList.AddRect(
            m_tileset.get_position(), m_tileset.get_end_position(),
            m_gridColor.to_integer() );

        // Horizontal lines of the grid
        for ( unsigned int x {
                  math::division_reminder_u( scrolling.x, TILE_PIXEL_SIZE ) };
              x < m_tileset.get_size().pixel().x; x += TILE_PIXEL_SIZE_U )
        {
            math::Vector2F const pointA {
                m_tileset.get_position()
                + math::Vector2F {static_cast< float >( x ), 0.f}
            };
            math::Vector2F const pointB {
                m_tileset.get_position().x + static_cast< float >( x ),
                m_tileset.get_end_position().y };

            drawList.AddLine( pointA, pointB, m_gridColor.to_integer() );
        }
        // Vertical lines of the grid
        for ( unsigned int y {
                  math::division_reminder_u( scrolling.y, TILE_PIXEL_SIZE ) };
              y < m_tileset.get_size().pixel().y; y += TILE_PIXEL_SIZE_U )
        {
            math::Vector2F const pointA {
                m_tileset.get_position().x,
                m_tileset.get_position().y + static_cast< float >( y ) };
            math::Vector2F const pointB {
                m_tileset.get_end_position().x,
                m_tileset.get_position().y + static_cast< float >( y ) };

            drawList.AddLine( pointA, pointB, m_gridColor.to_integer() );
        }
    }

    void Selector::update_selection( ImDrawList & drawList )
    {
        math::Vector2F const mousePosition { input::get_mouse_position() };

        /// @brief Check if the mouse is inside the tileset grid
        bool const isInSelection {
            ImGui::IsWindowHovered() && m_tileset.contain( mousePosition ) };

        if ( isInSelection )
        {
            // Calculate the position of the selection rectangle
            math::Vector2F const selectionPosition {
                m_tileset.get_tile_position( mousePosition ).pixel() };

            // Selection Rectangle
            drawList.AddRectFilled(
                selectionPosition, selectionPosition + TILE_PIXEL_SIZE_VECTOR,
                m_gridColor.to_integer() );

            std::stringstream outputSelection {};
            outputSelection
                << "Selection pixel position : " << selectionPosition << "\n";
            outputSelection
                << "Selection tile position : "
                << m_tileset.get_tile_position( mousePosition ).tile() << "\n";
            outputSelection << "Tileset Size : " << m_tileset.get_size().tile()
                            << "\n";
            unsigned int const tileHovered {
                m_tileset.get_tile_position( mousePosition ).value() };
            outputSelection << "Tile Hovered : " << tileHovered << "\n";
            if ( input::is_pressed( sf::Mouse::Button::Left ) )
            {
                outputSelection << "Button Pressed"
                                << "\n";
                m_tileSelected = tileHovered;
            }
            outputSelection << "Tile Position : "
                            << selectionPosition - m_tileset.get_position()
                            << "\n";
            ImGui::Text( "%s", outputSelection.str().c_str() );
        }

        std::stringstream output {};
        output << "Mouse Position : " << mousePosition << "\n";
        output << "Tilemap Position : " << m_tileset.get_position() << "\n";
        output << "Tilemap Size : " << m_tileset.get_size().pixel() << "\n";
        output << "Is In Selection ? : " << std::boolalpha << isInSelection
               << "\n";
        if ( m_tileSelected.has_value() )
        {
            output << "TILE SELECTED : " << m_tileSelected.value() << "\n";
        }
        else
        {
            output << "TILE SELECTED : None"
                   << "\n";
        }
        ImGui::Text( "%s", output.str().c_str() );
    }
}  // namespace tile