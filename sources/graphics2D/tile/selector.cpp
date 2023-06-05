#include "selector.hpp"

#include <sstream>                   // for operator<<, basic_ostream, stri...
#include <string>                    // for char_traits, allocator, basic_s...

#include <SFML/Graphics/Color.hpp>   // for Color
#include <SFML/Window/Mouse.hpp>     // for Mouse, Mouse::Button, Mouse::Left
#include <imgui/imgui-SFML.h>        // for Image
#include <imgui/imgui.h>             // for GetWindowDrawList, IsWindowHovered

#include "game/resources.hpp"        // for get_texture
#include "graphics2D/constants.hpp"  // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZE_U
#include "libraries/imgui.hpp"       // for table_to_sfml, to_integer, to_t...
#include "maths/geometry/line.hpp"
#include "maths/geometry/point.tpp"  // for Point::Point<Type>
#include "maths/numerics.hpp"        // for division_reminder_u
#include "maths/vector2.hpp"         // for Vector2F, Vector2, Vector2I
#include "maths/vector2.tpp"         // for operator<<, operator+, Vector2:...
                                     // for get_mouse_position, is_pressed

namespace tile
{
    Selector::Selector()
      // TODO have tilemap as a parameter
      : m_tileset { resource::tileset::get( "town.png" ) },
        m_subTilesetPosition {
            m_tileset.get_position( { 0, 0 }, tile::Position::Tile ) },
        m_subTilesetSize { math::min( m_tileset.get_size().tile(),
                                      math::Vector2U { 12, 12 } ),
                           tile::Size::Tile },
        m_tileSelected { std::nullopt },
        m_gridColor { Color::RGBA { 118, 118, 118, 255 } },
        m_show { .debug = false, .grid = true, .scrollbar = false }
    {}

    void Selector::update( UpdateContext & context )
    {
        if ( ImGui::BeginWindow( "Tile Selector" ) )
        {
            this->update_settings( context );

            math::Vector2U size { m_subTilesetSize.tile() };
            if ( ImGui::SliderVec( "TileSet Size", size, 4, 16 ) )
            {
                m_subTilesetSize.set_value( size, tile::Size::Tile );
            }

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
            if ( ! m_show.scrollbar )
            {
                windowFlags |= ImGuiWindowFlags_NoScrollbar;
            }
            if ( ImGui::BeginChild( "scrolling_texture",
                                    m_subTilesetSize.pixel().to_float(), true,
                                    windowFlags ) )
            {
                this->update_scroll( context );
                this->show_tileset();

                if ( m_show.grid )
                {
                    this->update_grid( context );
                }

                this->update_selection( context );
            }
            ImGui::EndChild();

            ImGui::Checkbox( "Show Debug Informations ?", &m_show.debug );
            if ( m_show.debug )
            {
                this->update_informations( context );
            }
        }
    }

    tile::Set const & Selector::get_tileset() const
    {
        return m_tileset;
    }

    std::optional< tile::Position > Selector::get_tile_selected() const
    {
        return m_tileSelected;
    }

    void Selector::update_settings( UpdateContext & /* context */ )
    {
        ImGui::Checkbox( "Enable Grid ?", &m_show.grid );
        ImGui::Checkbox( "Show Scrollbar ?", &m_show.scrollbar );
        if ( m_show.grid )
        {
            ImGui::ColorEdit4( "Grid Color", m_gridColor.to_table() );
        }
    }

    void Selector::update_grid( UpdateContext & /* context */ )
    {
        // Border
        ImGui::GetWindowDrawList()->AddRect( m_tileset.get_position(),
                                             m_tileset.get_bound_pos(),
                                             m_gridColor.to_integer() );

        // Horizontal lines
        for ( unsigned int x = 0; x < m_tileset.get_size().tile().x; ++x )
        {
            float       xPixel = static_cast< float >( x ) * TILE_PIXEL_SIZE;
            math::LineF line { { m_tileset.get_position().x + xPixel,
                                 m_tileset.get_position().y },
                               { m_tileset.get_position().x + xPixel,
                                 m_tileset.get_bound_pos().y } };

            ImGui::GetWindowDrawList()->AddLine( line.pointA, line.pointB,
                                                 m_gridColor.to_integer() );
        }
        // Vertical lines
        for ( unsigned int y = 0; y < m_tileset.get_size().tile().y; ++y )
        {
            float       yPixel = static_cast< float >( y ) * TILE_PIXEL_SIZE;
            math::LineF line { { m_tileset.get_position().x,
                                 m_tileset.get_position().y + yPixel },
                               { m_tileset.get_bound_pos().x,
                                 m_tileset.get_position().y + yPixel } };

            ImGui::GetWindowDrawList()->AddLine( line.pointA, line.pointB,
                                                 m_gridColor.to_integer() );
        }
    }

    void Selector::update_scroll( UpdateContext & /* context */ )
    {
        m_subTilesetPosition = tile::Position { ImGui::GetScroll().to_u_int(),
                                                m_subTilesetSize,
                                                tile::Position::Pixel };
    }

    void Selector::update_selection( UpdateContext & context )
    {
        math::Vector2I mousePosition { context.inputs.get_mouse_position() };

        if ( ImGui::IsWindowHovered()
             && mousePosition.to_point().to_u_int().is_inside(
                 m_subTilesetPosition.pixel(), m_subTilesetSize.pixel() ) )
        {  // the mouse is inside the tileset grid
            tile::Position selectionPosition { m_tileset.get_position(
                mousePosition.to_point().to_float()
                    - m_tileset.get_position().to_point(),
                tile::Position::Pixel ) };

            // Selection Rectangle
            ImGui::GetWindowDrawList()->AddRectFilled(
                selectionPosition.pixel().to_float() + m_tileset.get_position(),
                selectionPosition.pixel().to_float() + m_tileset.get_position()
                    + TILE_PIXEL_SIZE_VECTOR,
                m_gridColor.to_integer() );

            if ( context.inputs.is_pressed( sf::Mouse::Button::Left ) )
            {
                m_tileSelected = selectionPosition;
            }
        }
    }

    void Selector::show_tileset()
    {
        m_tileset.set_position( ImGui::GetCursorScreenPos() );
        ImGui::Image( m_tileset.get_texture() );
    }

    void Selector::update_informations( UpdateContext & context )
    {
        std::ostringstream output {};
        output << "Mouse Position : " << context.inputs.get_mouse_position()
               << "\n";
        output << "Tileset Position : " << m_tileset.get_position() << "\n";
        output << "Tileset Size : " << m_tileset.get_size().tile() << "\n";
        output << "SubTileset Position : " << m_subTilesetPosition.pixel()
               << "\n";
        output << "SubTileset Size : " << m_subTilesetSize.tile() << "\n";
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
}  // namespace tile
