#include "tile_selector.hpp"

#include <iostream>
#include <sstream>

#include <cmath>

#include "input/input.hpp"
#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "main/window.hpp"
#include "maths/maths.hpp"
#include "tools/global_variable.hpp"

TileSelector::TileSelector()
  : m_tileset( resources::get_texture( "ground.png" ) ),
    m_tileSelected( -1 ),
    m_isGridEnabled( true ),
    m_gridColorTable()
{
    sf::Color gridColor { 118, 118, 118, 255 };
    ImGui::color::to_table( gridColor, this->m_gridColorTable );
}

Tileset const & TileSelector::get_tileset() const
{
    return this->m_tileset;
}

int TileSelector::get_tile_selected() const
{
    return this->m_tileSelected;
}

void TileSelector::update_before( float /* deltaTime */ )
{
    if ( ImGui::Begin( "Tile Selector" ) )
    {
        ImGui::Checkbox( "Enable grid", &this->m_isGridEnabled );
        ImGui::ColorEdit4( "Color Edit", this->m_gridColorTable );

        this->m_tileset.set_position( ImGui::GetCursorScreenPos() );
        ImGui::Image( this->m_tileset.get_texture() );

        if ( this->m_isGridEnabled )
        {
            this->update_grid( *ImGui::GetWindowDrawList() );
        }
        this->update_selection( *ImGui::GetWindowDrawList() );
    }
    ImGui::End();
}

void TileSelector::update_grid( ImDrawList & drawList )
{
    math::Vector2F const scrolling { 0.f, 0.f };

    // Tilemap Border
    drawList.AddRect( this->m_tileset.get_position(),
                      this->m_tileset.get_end_position(),
                      ImGui::color::to_integer( ImGui::color::table_to_sfml(
                          this->m_gridColorTable ) ) );

    // Horizontal lines of the grid
    for ( unsigned int x {
              math::division_reminder_u( scrolling.x, TILE_PIXEL_SIZE ) };
          x < this->m_tileset.get_size_in_pixel().x;
          x += TILE_PIXEL_SIZE_U )
    {
        math::Vector2F const pointA {
            this->m_tileset.get_position()
            + math::Vector2F {static_cast< float >( x ), 0.f}
        };
        math::Vector2F const pointB { this->m_tileset.get_position().x
                                          + static_cast< float >( x ),
                                      this->m_tileset.get_end_position().y };

        drawList.AddLine( pointA,
                          pointB,
                          ImGui::color::to_integer( ImGui::color::table_to_sfml(
                              this->m_gridColorTable ) ) );
    }
    // Vertical lines of the grid
    for ( unsigned int y {
              math::division_reminder_u( scrolling.y, TILE_PIXEL_SIZE ) };
          y < this->m_tileset.get_size_in_pixel().y;
          y += TILE_PIXEL_SIZE_U )
    {
        math::Vector2F const pointA { this->m_tileset.get_position().x,
                                      this->m_tileset.get_position().y
                                          + static_cast< float >( y ) };
        math::Vector2F const pointB { this->m_tileset.get_end_position().x,
                                      this->m_tileset.get_position().y
                                          + static_cast< float >( y ) };

        drawList.AddLine( pointA,
                          pointB,
                          ImGui::color::to_integer( ImGui::color::table_to_sfml(
                              this->m_gridColorTable ) ) );
    }
}
void TileSelector::update_selection( ImDrawList & drawList )
{
    math::Vector2F const mousePosition { input::get_mouse_position() };
    bool const isInSelection { ImGui::IsWindowHovered()
                               && this->m_tileset.contain( mousePosition ) };
    if ( isInSelection )
    {
        // Calculate the position of the selection rectangle
        math::Vector2F const selectionPosition {
            this->m_tileset.get_tile_position_in_pixel( mousePosition, false )
        };

        // Selection Rectangle
        drawList.AddRectFilled(
            selectionPosition,
            selectionPosition + TILE_PIXEL_SIZE_VECTOR,
            ImGui::color::to_integer(
                ImGui::color::table_to_sfml( this->m_gridColorTable ) ) );

        std::stringstream outputSelection {};
        outputSelection << "Selection pixel position : " << selectionPosition
                        << "\n";
        outputSelection << "Selection tile position : "
                        << this->m_tileset.get_tile_position_in_tile(
                               mousePosition,
                               false )
                        << "\n";
        outputSelection << "Tileset Size : "
                        << this->m_tileset.get_size_in_tile() << "\n";
        int const tileHovered {
            this->m_tileset.get_tile_value_from_pixel_position( mousePosition,
                                                                false )
        };
        outputSelection << "Tile Hovered : " << tileHovered << "\n";
        if ( input::is_pressed( sf::Mouse::Button::Left ) )
        {
            outputSelection << "Button Pressed"
                            << "\n";
            this->m_tileSelected = tileHovered;
        }
        outputSelection << "Tile Position : "
                        << selectionPosition - this->m_tileset.get_position()
                        << "\n";
        ImGui::Text( "%s", outputSelection.str().c_str() );
    }

    std::stringstream output {};
    output << "Mouse Position : " << mousePosition << "\n";
    output << "Tilemap Position : " << this->m_tileset.get_position() << "\n";
    output << "Tilemap Size : " << this->m_tileset.get_size_in_pixel() << "\n";
    output << "Is In Selection ? : " << std::boolalpha << isInSelection << "\n";
    output << "TILE SELECTED : " << this->m_tileSelected << "\n";
    ImGui::Text( "%s", output.str().c_str() );
}