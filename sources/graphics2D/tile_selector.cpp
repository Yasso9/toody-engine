#include "tile_selector.hpp"

#include <iostream>
#include <sstream>

#include <cmath>

#include "main/resources.hpp"
#include "tools/global_variable.hpp"
#include "tools/imgui.hpp"
#include "tools/maths.hpp"

uint32_t to_integer_imgui_color( sf::Color const & color )
{
    return ( static_cast< ImU32 >( color.a ) << 24 )
           | ( static_cast< ImU32 >( color.b ) << 16 )
           | ( static_cast< ImU32 >( color.g ) << 8 )
           | ( static_cast< ImU32 >( color.r ) << 0 );
}

void sfml_to_table_color( sf::Color const & sfmlColor, float tableColor[4] )
{
    // TYPO : assert that sfmlColor values are under 255

    tableColor[0] = static_cast< float >( sfmlColor.r ) / COLOR_RANGE;
    tableColor[1] = static_cast< float >( sfmlColor.g ) / COLOR_RANGE;
    tableColor[2] = static_cast< float >( sfmlColor.b ) / COLOR_RANGE;
    tableColor[3] = static_cast< float >( sfmlColor.a ) / COLOR_RANGE;
}

sf::Color table_to_sfml_color( float const tableColor[4] )
{
    // TYPO : assert that sfmlColor values are under 1 and over 0

    return { static_cast< sf::Uint8 >( tableColor[0] * COLOR_RANGE ),
             static_cast< sf::Uint8 >( tableColor[1] * COLOR_RANGE ),
             static_cast< sf::Uint8 >( tableColor[2] * COLOR_RANGE ),
             static_cast< sf::Uint8 >( tableColor[3] * COLOR_RANGE ) };
}

TileSelector::TileSelector()
  : m_tileset( Resources::get_instance().get_texture(
      Resources::E_TextureKey::Tileset ) ),
    m_isGridEnabled( true ),
    m_gridColorTable()
{
    sf::Color gridColor { 118, 118, 118, 255 };
    sfml_to_table_color( gridColor, this->m_gridColorTable );
}

Tileset const & TileSelector::get_tileset() const
{
    return this->m_tileset;
}

void TileSelector::update()
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
    math::Vector2D const scrolling { 0.f, 0.f };

    // Tilemap Border
    drawList.AddRect( this->m_tileset.get_position(),
                      this->m_tileset.get_end_position(),
                      to_integer_imgui_color(
                          table_to_sfml_color( this->m_gridColorTable ) ) );

    // Horizontal lines of the grid
    for ( float x = fmodf( scrolling.x, TILE_PIXEL_SIZE );
          x < this->m_tileset.get_size_in_pixel().x;
          x += TILE_PIXEL_SIZE )
    {
        math::Vector2D const pointA {
            this->m_tileset.get_position() + math::Vector2D {x, 0.f}
        };
        math::Vector2D const pointB { this->m_tileset.get_position().x + x,
                                      this->m_tileset.get_end_position().y };

        drawList.AddLine( pointA,
                          pointB,
                          to_integer_imgui_color(
                              table_to_sfml_color( this->m_gridColorTable ) ) );
    }
    // Vertical lines of the grid
    for ( float y = fmodf( scrolling.y, TILE_PIXEL_SIZE );
          y < this->m_tileset.get_size_in_pixel().y;
          y += TILE_PIXEL_SIZE )
    {
        math::Vector2D const pointA { this->m_tileset.get_position().x,
                                      this->m_tileset.get_position().y + y };
        math::Vector2D const pointB { this->m_tileset.get_end_position().x,
                                      this->m_tileset.get_position().y + y };

        drawList.AddLine( pointA,
                          pointB,
                          to_integer_imgui_color(
                              table_to_sfml_color( this->m_gridColorTable ) ) );
    }
}
void TileSelector::update_selection( ImDrawList & drawList )
{
    math::Vector2D const mousePosition { ImGui::GetMousePos() };
    bool isInSelection { ImGui::IsWindowHovered()
                         && this->m_tileset.contain( mousePosition ) };
    if ( isInSelection )
    {
        // Calculate the position of the selection rectangle
        math::Vector2D const selectionPosition {
            this->m_tileset.get_tile_position_in_pixel( mousePosition, false )
        };

        // Selection Rectangle
        drawList.AddRectFilled( selectionPosition,
                                selectionPosition + TILE_PIXEL_SIZE_VECTOR,
                                to_integer_imgui_color( table_to_sfml_color(
                                    this->m_gridColorTable ) ) );

        std::stringstream outputSelection {};
        outputSelection << "Selection Rect : " << selectionPosition << "\n";
        outputSelection << "Selection Tile : "
                        << this->m_tileset.get_tile_position_in_tile(
                               mousePosition,
                               false )
                        << "\n";
        outputSelection << "Tileset Size : "
                        << this->m_tileset.get_size_in_tile() << "\n";
        outputSelection << "Tile Selected : "
                        << this->m_tileset.get_tile_value_from_pixel_position(
                               mousePosition,
                               false )
                        << "\n";
        ImGui::Text( "%s", outputSelection.str().c_str() );
    }
    std::stringstream output {};
    output << "Mouse Position : " << mousePosition << "\n";
    output << "Tilemap Position : " << this->m_tileset.get_position() << "\n";
    output << "Tilemap Size : " << this->m_tileset.get_size_in_pixel() << "\n";
    output << "Is In Selection ? : " << std::boolalpha << isInSelection << "\n";
    ImGui::Text( "%s", output.str().c_str() );
}