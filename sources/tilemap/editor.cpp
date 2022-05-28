#include "editor.hpp"

#include <iostream>
#include <sstream>

#include <cmath>

#include "main/resources.hpp"
#include "tools/imgui.hpp"

uint32_t to_integer_imgui_color( sf::Color const & color )
{
    return ( static_cast< ImU32 >( color.a ) << 24 )
           | ( static_cast< ImU32 >( color.b ) << 16 )
           | ( static_cast< ImU32 >( color.g ) << 8 )
           | ( static_cast< ImU32 >( color.r ) << 0 );
}

static constexpr unsigned int COLOR_RANGE { 255u };

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

TileMapEditor::TileMapEditor() : m_isGridEnabled( true )
{
    sf::Color gridColor { 118, 118, 118, 255 };
    sfml_to_table_color( gridColor, this->m_gridColorTable );
}

void TileMapEditor::update()
{
    if ( ImGui::Begin( "Tilemap" ) )
    {
        ImGui::Checkbox( "Enable grid", &this->m_isGridEnabled );

        sf::Texture const & tilemapTexture {
            Resources::get_instance().get_texture(
                Resources::E_TextureKey::Tileset )
        };

        ImGui::ColorEdit4( "Color Edit", this->m_gridColorTable );

        sf::Vector2f tilemapPosition { ImGui::GetCursorScreenPos() };
        sf::Vector2f tilemapSize { tilemapTexture.getSize() };
        sf::Vector2f tilemapEndPosition { tilemapPosition + tilemapSize };

        ImDrawList * drawList { ImGui::GetWindowDrawList() };

        ImGui::Image( tilemapTexture );
        sf::Vector2f scrolling { 0.f, 0.f };

        // Draw grid + all lines in the canvas
        // drawList->PushClipRect( tilemapPosition, tilemapEndPosition, true );
        if ( this->m_isGridEnabled )
        {
            // Tilemap Border
            drawList->AddRect( tilemapPosition,
                               tilemapEndPosition,
                               to_integer_imgui_color( table_to_sfml_color(
                                   this->m_gridColorTable ) ) );

            constexpr float GRID_STEP = 32.0f;
            // Horizontal lines of the grid
            for ( float x = fmodf( scrolling.x, GRID_STEP ); x < tilemapSize.x;
                  x += GRID_STEP )
            {
                drawList->AddLine(
                    ImVec2( tilemapPosition.x + x, tilemapPosition.y ),
                    ImVec2( tilemapPosition.x + x, tilemapEndPosition.y ),
                    to_integer_imgui_color(
                        table_to_sfml_color( this->m_gridColorTable ) ) );
            }
            // Vertical lines of the grid
            for ( float y = fmodf( scrolling.y, GRID_STEP ); y < tilemapSize.y;
                  y += GRID_STEP )
            {
                drawList->AddLine(
                    ImVec2( tilemapPosition.x, tilemapPosition.y + y ),
                    ImVec2( tilemapEndPosition.x, tilemapPosition.y + y ),
                    to_integer_imgui_color(
                        table_to_sfml_color( this->m_gridColorTable ) ) );
            }
        }
        // drawList->PopClipRect();

        ImGui::End();
    }
}