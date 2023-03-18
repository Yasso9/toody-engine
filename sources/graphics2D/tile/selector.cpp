#include "selector.hpp"

#include <sstream>  // for operator<<, basic_ostream, stri...
#include <string>   // for char_traits, allocator, basic_s...

#include <IMGUI/imgui-SFML.h>       // for Image
#include <IMGUI/imgui.h>            // for GetWindowDrawList, IsWindowHovered
#include <SFML/Graphics/Color.hpp>  // for Color
#include <SFML/Window/Mouse.hpp>    // for Mouse, Mouse::Button, Mouse::Left

#include "game/resources.hpp"        // for get_texture
#include "graphics2D/constants.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZE_U
#include "libraries/imgui.hpp"       // for table_to_sfml, to_integer, to_t...
#include "maths/geometry/line.hpp"
#include "maths/geometry/point.tpp"  // for Point::Point<Type>
#include "maths/numerics.hpp"        // for division_reminder_u
#include "maths/vector2.hpp"         // for Vector2F, Vector2, Vector2I
#include "maths/vector2.tpp"         // for operator<<, operator+, Vector2:...
#include "states/input.hpp"          // for get_mouse_position, is_pressed
#include "tools/path.hpp"

namespace tile
{
    Selector::Selector()
      : m_tileset { resources::get_texture( path::get_folder( path::Tilesets )
                                            / "tilemap.png" ) },
        m_tileSelected { std::nullopt },
        m_gridColor { Color::RGBA { 118, 118, 118, 255 } }
    {}

    tile::Set const & Selector::get_tileset() const
    {
        return m_tileset;
    }

    std::optional< tile::Position > Selector::get_tile_selected() const
    {
        return m_tileSelected;
    }

    void Selector::update( UpdateContext context )
    {
        ImGui::P_Show(
            "Tile Selector", NULL, ImGuiWindowFlags_AlwaysAutoResize, [&] () {
                static bool             isGridEnabled { true };
                static math::RectangleI gridBorder {
                    math::Vector2I { 0, 0 }, math::Vector2I { 384, 384 } };
                static math::Vector2F scroll { 0.f, 0.f };
                math::Vector2F        addedScroll { 0.f, 0.f };
                math::Vector2F        maxScrollGrid { 0.f, 0.f };

                {  // UPDATE TILEMAP
                    ImGui::Checkbox( "Enable grid ?", &isGridEnabled );
                    if ( isGridEnabled )
                    {
                        ImGui::ColorEdit4( "Grid Color",
                                           m_gridColor.to_table() );
                    }

                    if ( ImGui::SmallButton( "<<" ) )
                    {
                        addedScroll.x -= TILE_PIXEL_SIZE;
                    }
                    ImGui::SameLine();
                    if ( ImGui::SmallButton( ">>" ) )
                    {
                        addedScroll.x += TILE_PIXEL_SIZE;
                    }

                    static math::Vector2I gridSizeTile { 12, 12 };
                    ImGui::SliderInt( "Columns", &gridSizeTile.x, 4, 16 );
                    ImGui::SliderInt( "Lines", &gridSizeTile.y, 4, 16 );
                    gridBorder.size = gridSizeTile * TILE_PIXEL_SIZE_I;
                    if ( ImGui::BeginChild( "scrolling_texture",
                                            gridBorder.size.to_float(), false,
                                            ImGuiWindowFlags_NoScrollbar ) )
                    {
                        {  // UPDATE SCROLL
                            math::Vector2I scrollMultipler { 0, 0 };

                            if ( scroll.x > ImGui::GetScrollX() )
                            {
                                scrollMultipler.x = 1;
                            }
                            else if ( scroll.x < ImGui::GetScrollX() )
                            {
                                scrollMultipler.x = -1;
                            }
                            if ( scroll.y > ImGui::GetScrollY() )
                            {
                                scrollMultipler.y = -1;
                            }
                            else if ( scroll.y < ImGui::GetScrollY() )
                            {
                                scrollMultipler.y = 1;
                            }
                            ImGui::SetScrollX(
                                addedScroll.x + ImGui::GetScrollX()
                                - static_cast< float >(
                                    scrollMultipler.x
                                    * static_cast< int >( ImGui::GetScrollX() )
                                    % 32 ) );
                            ImGui::SetScrollY(
                                addedScroll.y + ImGui::GetScrollY()
                                - static_cast< float >(
                                    scrollMultipler.y
                                    * static_cast< int >( ImGui::GetScrollY() )
                                    % 32 ) );

                            scroll        = { ImGui::GetScrollX(),
                                              ImGui::GetScrollY() };
                            maxScrollGrid = { ImGui::GetScrollMaxX(),
                                              ImGui::GetScrollMaxY() };
                        }

                        {  // UDPATE TILESET
                            gridBorder.position =
                                math::Vector2I { ImGui::GetCursorScreenPos() }
                                + scroll.to_int();

                            ImGui::Image( m_tileset.get_texture() );
                        }

                        {  // UPDATE GRID
                            if ( isGridEnabled )
                            {  // Draw grid
                                // Border
                                ImGui::GetWindowDrawList()->AddRect(
                                    gridBorder.position.to_float(),
                                    gridBorder.end_position().to_float(),
                                    m_gridColor.to_integer() );

                                // Horizontal lines
                                for ( int x = 0; x < gridBorder.size.x;
                                      x += TILE_PIXEL_SIZE_I )
                                {
                                    math::LineI line {
                                        { gridBorder.position.x + x,
                                          gridBorder.position.y },
                                        { gridBorder.position.x + x,
                                          gridBorder.end_position().y } };

                                    ImGui::GetWindowDrawList()->AddLine(
                                        line.pointA.to_float(),
                                        line.pointB.to_float(),
                                        m_gridColor.to_integer() );
                                }
                                // Vertical lines
                                for ( int y = 0; y < gridBorder.size.y;
                                      y += TILE_PIXEL_SIZE_I )
                                {
                                    math::LineI line {
                                        { gridBorder.position.x,
                                          gridBorder.position.y + y },
                                        { gridBorder.end_position().x,
                                          gridBorder.position.y + y } };

                                    ImGui::GetWindowDrawList()->AddLine(
                                        line.pointA.to_float(),
                                        line.pointB.to_float(),
                                        m_gridColor.to_integer() );
                                }
                            }
                        }

                        {  // UPDATE SELECTION RECT
                            math::Vector2F mousePosition {
                                input::get_mouse_position( context.window ) };

                            if ( ImGui::IsWindowHovered()
                                 && gridBorder.contain(
                                     mousePosition.to_point().to_int() ) )
                            {  // the mouse is inside the tileset grid
                                tile::Position selectionPosition {
                                    mousePosition.to_u_int()
                                        - gridBorder.position,
                                    m_tileset.get_size(),
                                    tile::Position::Pixel };

                                // Selection Rectangle
                                ImGui::GetWindowDrawList()->AddRectFilled(
                                    selectionPosition.pixel().to_float()
                                        + gridBorder.position,
                                    selectionPosition.pixel().to_float()
                                        + gridBorder.position
                                        + TILE_PIXEL_SIZE_VECTOR,
                                    m_gridColor.to_integer() );

                                if ( input::is_pressed(
                                         context.window,
                                         sf::Mouse::Button::Left ) )
                                {
                                    // There's a click, the tile selected change
                                    // value
                                    m_tileSelected = tile::Position {
                                        selectionPosition.pixel()
                                            + scroll.to_u_int(),
                                        m_tileset.get_size(),
                                        tile::Position::Pixel };
                                }
                            }
                        }
                    }
                    ImGui::EndChild();
                }

                {  // UPDATE DEBUG
                    static bool showDebug { true };
                    ImGui::Checkbox( "Show Debug Informations ?", &showDebug );
                    if ( showDebug )
                    {
                        std::ostringstream output {};
                        output << "Mouse Position : "
                               << input::get_mouse_position( context.window )
                               << "\n";
                        output << "Grid : " << gridBorder << "\n";
                        output << "Grid Scroll : " << scroll << "\n";
                        output << "Grid Max Scroll : " << maxScrollGrid << "\n";
                        output
                            << "Tileset Position : " << m_tileset.get_position()
                            << "\n";
                        output << "Tileset Size (Pixel) : "
                               << m_tileset.get_size().pixel() << "\n";
                        output << "Tileset Size (Tile): "
                               << m_tileset.get_size().tile() << "\n";
                        output << "Current Tile Selected : ";
                        if ( m_tileSelected.has_value() )
                        {
                            output << m_tileSelected.value().value() << "\n";
                        }
                        else
                        {
                            output << "None"
                                   << "\n";
                        }
                        ImGui::Text( "%s", output.str().c_str() );
                    }
                }
            } );
    }
}  // namespace tile