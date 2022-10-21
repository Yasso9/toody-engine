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

/// @todo put theses functions on the ImGUI::grid namespace
static void draw_grid (
    ImDrawList & drawList, Tileset const & tileset, Color gridColor );
static void draw_horizontal_lines_grid (
    ImDrawList & drawList, Tileset const & tileset, Color lineColor );
static void draw_vertical_lines_grid (
    ImDrawList & drawList, Tileset const & tileset, Color lineColor );

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

    void Selector::update( float /* deltaTime */ )
    {
        ImGui::P_Show( "Tile Selector", [&] () {
            ImGui::Checkbox( "Enable grid ?", &m_isGridEnabled );
            ImGui::ColorEdit4( "Grid Color", m_gridColor.to_table() );

            m_tileset.set_position( ImGui::GetCursorScreenPos() );
            ImGui::Image( m_tileset.get_texture() );

            if ( m_isGridEnabled )
            {
                draw_grid(
                    *ImGui::GetWindowDrawList(), m_tileset, m_gridColor );
            }

            this->update_selection( *ImGui::GetWindowDrawList() );
        } );
    }

    void Selector::update_selection( ImDrawList & drawList )
    {
        math::Vector2F const mousePosition { input::get_mouse_position() };

        /// @brief Check if the mouse is inside the tileset grid
        bool const isInSelection {
            ImGui::IsWindowHovered() && m_tileset.contain( mousePosition ) };

        if ( isInSelection )
        {
            // Position of the selection rectangle depending of the
            // tileset position
            tile::Position const selectionPosition {
                m_tileset.get_tile_position(
                    mousePosition - m_tileset.get_position() ) };

            // Selection Rectangle
            drawList.AddRectFilled(
                selectionPosition.pixel().to_float() + m_tileset.get_position(),
                selectionPosition.pixel().to_float() + TILE_PIXEL_SIZE_VECTOR
                    + m_tileset.get_position(),
                m_gridColor.to_integer() );

            std::stringstream outputSelection {};
            outputSelection
                << "Selection pixel position : " << selectionPosition.pixel()
                << "\n";
            outputSelection
                << "Selection tile position : " << selectionPosition.tile()
                << "\n";
            outputSelection
                << "Selection Tile Value : " << selectionPosition.value()
                << "\n";

            if ( input::is_pressed( sf::Mouse::Button::Left ) )
            {
                // There's a click, the tile selected change value
                outputSelection << "Button Pressed !"
                                << "\n";
                m_tileSelected = selectionPosition.value();
            }
            ImGui::Text( "%s", outputSelection.str().c_str() );
        }

        std::stringstream output {};
        output << "Mouse Position : " << mousePosition << "\n";
        output << "Tileset Position : " << m_tileset.get_position() << "\n";
        output << "Tileset Size (Pixel) : " << m_tileset.get_size().pixel()
               << "\n";
        output << "Tileset Size (Tile): " << m_tileset.get_size().tile()
               << "\n";
        output << "Is In Selection ? : " << std::boolalpha << isInSelection
               << "\n";
        output << "Current Tile Selected : ";
        if ( m_tileSelected.has_value() )
        {
            output << m_tileSelected.value() << "\n";
        }
        else
        {
            output << "None"
                   << "\n";
        }
        ImGui::Text( "%s", output.str().c_str() );
    }
}  // namespace tile

static void draw_grid (
    ImDrawList & drawList, Tileset const & tileset, Color gridColor )
{
    // Tilemap Border
    drawList.AddRect(
        tileset.get_position(), tileset.get_end_position(),
        gridColor.to_integer() );

    draw_horizontal_lines_grid( drawList, tileset, gridColor );
    draw_vertical_lines_grid( drawList, tileset, gridColor );
}

static void draw_horizontal_lines_grid (
    ImDrawList & drawList, Tileset const & tileset, Color lineColor )
{
    for ( unsigned int x = 0u; x < tileset.get_size().pixel().x;
          x += TILE_PIXEL_SIZE_U )
    {
        math::Vector2F const pointA {
            tileset.get_position().x + static_cast< float >( x ),
            tileset.get_position().y };
        math::Vector2F const pointB {
            tileset.get_position().x + static_cast< float >( x ),
            tileset.get_end_position().y };

        drawList.AddLine( pointA, pointB, lineColor.to_integer() );
    }
}

static void draw_vertical_lines_grid (
    ImDrawList & drawList, Tileset const & tileset, Color lineColor )
{
    for ( unsigned int y = 0u; y < tileset.get_size().pixel().y;
          y += TILE_PIXEL_SIZE_U )
    {
        math::Vector2F const pointA {
            tileset.get_position().x,
            tileset.get_position().y + static_cast< float >( y ) };
        math::Vector2F const pointB {
            tileset.get_end_position().x,
            tileset.get_position().y + static_cast< float >( y ) };

        drawList.AddLine( pointA, pointB, lineColor.to_integer() );
    }
}