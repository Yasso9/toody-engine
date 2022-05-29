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

TileSelector::TileSelector() : m_isGridEnabled( true ), m_gridColorTable()
{
    sf::Color gridColor { 118, 118, 118, 255 };
    sfml_to_table_color( gridColor, this->m_gridColorTable );
}

void TileSelector::update()
{
    if ( ImGui::Begin( "Tile Selector" ) )
    {
        ImGui::Checkbox( "Enable grid", &this->m_isGridEnabled );

        sf::Texture const & tilemapTexture {
            Resources::get_instance().get_texture(
                Resources::E_TextureKey::Tileset )
        };

        ImGui::ColorEdit4( "Color Edit", this->m_gridColorTable );

        sf::Vector2f const tilemapPosition { ImGui::GetCursorScreenPos() };
        sf::Vector2f const tilemapSize { tilemapTexture.getSize() };
        sf::Vector2f const tilemapEndPosition { tilemapPosition + tilemapSize };

        ImDrawList * drawList { ImGui::GetWindowDrawList() };

        ImGui::Image( tilemapTexture );
        sf::Vector2f scrolling { 0.f, 0.f };

        if ( this->m_isGridEnabled )
        {
            // Tilemap Border
            drawList->AddRect( tilemapPosition,
                               tilemapEndPosition,
                               to_integer_imgui_color( table_to_sfml_color(
                                   this->m_gridColorTable ) ) );

            // Horizontal lines of the grid
            for ( float x = fmodf( scrolling.x, TILE_PIXEL_SIZE );
                  x < tilemapSize.x;
                  x += TILE_PIXEL_SIZE )
            {
                drawList->AddLine(
                    ImVec2( tilemapPosition.x + x, tilemapPosition.y ),
                    ImVec2( tilemapPosition.x + x, tilemapEndPosition.y ),
                    to_integer_imgui_color(
                        table_to_sfml_color( this->m_gridColorTable ) ) );
            }
            // Vertical lines of the grid
            for ( float y = fmodf( scrolling.y, TILE_PIXEL_SIZE );
                  y < tilemapSize.y;
                  y += TILE_PIXEL_SIZE )
            {
                drawList->AddLine(
                    ImVec2( tilemapPosition.x, tilemapPosition.y + y ),
                    ImVec2( tilemapEndPosition.x, tilemapPosition.y + y ),
                    to_integer_imgui_color(
                        table_to_sfml_color( this->m_gridColorTable ) ) );
            }
        }

        bool isInSelection { false };

        math::Vector2D const mousePosition { ImGui::GetMousePos() };

        if ( ImGui::IsWindowHovered()
             && mousePosition.is_contained( tilemapPosition, tilemapSize ) )
        {
            isInSelection = true;

            // Tile const tile( tilemapPosition, mousePosition );

            math::Vector2D selectionTilePosition {};
            selectionTilePosition.x = std::floor(
                ( mousePosition.x - tilemapPosition.x ) / TILE_PIXEL_SIZE );
            selectionTilePosition.y = std::floor(
                ( mousePosition.y - tilemapPosition.y ) / TILE_PIXEL_SIZE );

            sf::Vector2f maxNumberOfTile {};
            maxNumberOfTile.x =
                std::floor( ( tilemapEndPosition.x - tilemapPosition.x )
                            / TILE_PIXEL_SIZE );
            maxNumberOfTile.y =
                std::floor( ( tilemapEndPosition.y - tilemapPosition.y )
                            / TILE_PIXEL_SIZE );

            int tileSelected { static_cast< int >(
                selectionTilePosition.x
                + selectionTilePosition.y * maxNumberOfTile.x ) };

            // TYPO ASSERT that tileSelected is >0 and <(maxNumberOfTile.x * maxNumberOfTile.y)

            // Calculate the position of the selection rectangle
            math::Vector2D selectionRectPosition {
                selectionTilePosition
                    * sf::Vector2f {TILE_PIXEL_SIZE, TILE_PIXEL_SIZE}
                + tilemapPosition
            };

            // Selection Rectangle
            drawList->AddRectFilled(
                selectionRectPosition.to_sfml_vector2f(),
                selectionRectPosition.to_sfml_vector2f()
                    + sf::Vector2f { TILE_PIXEL_SIZE, TILE_PIXEL_SIZE },
                to_integer_imgui_color(
                    table_to_sfml_color( this->m_gridColorTable ) ) );

            std::stringstream outputSelection {};
            outputSelection << "Mouse Position : " << mousePosition << "\n";
            outputSelection << "Selection Rect : " << selectionRectPosition
                            << "\n";
            outputSelection << "Selection Tile : " << selectionTilePosition
                            << "\n";
            outputSelection << "Max Tile : " << maxNumberOfTile << "\n";
            outputSelection << "Tile Selected : " << tileSelected << "\n";
            ImGui::Text( "%s", outputSelection.str().c_str() );
        }
        std::stringstream output {};
        output << "Tilemap Position : " << tilemapPosition << "\n";
        output << "Tilemap Size : " << tilemapSize << "\n";
        output << "Is In Selection ? : " << std::boolalpha << isInSelection
               << "\n";
        ImGui::Text( "%s", output.str().c_str() );
    }
    ImGui::End();
}