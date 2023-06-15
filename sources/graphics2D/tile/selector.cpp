#include "selector.hpp"

#include <sstream>  // for operator<<, basic_ostream, stri...
#include <string>   // for char_traits, allocator, basic_s...

#include <SFML/Graphics/Color.hpp>  // for Color
#include <SFML/Window/Mouse.hpp>    // for Mouse, Mouse::Button, Mouse::Left
#include <imgui/imgui-SFML.h>       // for Image
#include <imgui/imgui.h>            // for GetWindowDrawList, IsWindowHovered

#include "application/resources.hpp"  // for get_texture
#include "graphics2D/constants.hpp"   // for TILE_PIXEL_SIZE, TILE_PIXEL_SIZE_U
#include "imgui/imgui.hpp"            // for table_to_sfml, to_integer, to_t...
#include "maths/geometry/line.hpp"
#include "maths/geometry/point.tpp"  // for Point::Point<Type>
#include "maths/numerics.hpp"        // for division_reminder_u
#include "maths/vector2.hpp"         // for Vector2F, Vector2, Vector2I
#include "maths/vector2.tpp"         // for operator<<, operator+, Vector2:...
#include "tools/traces.hpp"          // for Trace::Warning

namespace tile
{
    SubTileset::SubTileset( tile::Set const &  tileset,
                            tile::Size const & maxSize )
      : m_position {},
        m_size { math::min( tileset.get_size().tile(), maxSize.tile() ),
                 tile::Type::Tile },
        m_firstTile { { 0, 0 }, m_size, tile::Type::Tile }
    {}

    math::Vector2F SubTileset::get_position() const
    {
        return m_position;
    }

    void SubTileset::set_position( math::Vector2F const & pos )
    {
        m_position = pos;
    }

    tile::Size SubTileset::get_size() const
    {
        return m_size;
    }

    tile::CellPos SubTileset::first_tile() const
    {
        return m_firstTile;
    }

    void SubTileset::set_first_tile( math::Vector2F const & scroll )
    {
        m_firstTile = tile::CellPos { scroll.to_uint(), m_size,
                                      tile::Type::Pixel };
    }

    void SubTileset::size_slider()
    {
        math::Vector2U size { m_size.tile() };
        if ( ImGui::SliderVec( "TileSet Size", size, 4, 16 ) )
        {
            m_size.tile( size );
        }
    }

    Selector::Selector( std::shared_ptr< tile::Set const > tileset )
      : SubWindow { "Tile Selector" },
        m_tileset { tileset },
        m_subTileset { *m_tileset, { 12, 12, tile::Type::Tile } },
        m_tileSelected { std::nullopt },
        m_gridColor { Color::RGBA { 118, 118, 118, 255 } },
        m_showElements { .grid = true },
        m_debugWindow { "Selector Informations" }
    {
        this->add_child( m_debugWindow );

        this->show_window();
    }

    void Selector::update_window( UpdateContext & context )
    {
        this->update_config( context );
        m_subTileset.size_slider();

        if ( ImGui::BeginChild( "scrolling_texture",
                                m_subTileset.get_size().pixel().to_float(),
                                true, ImGuiWindowFlags_NoScrollbar ) )
        {
            this->update_scroll( context );

            this->update_image( context );

            if ( m_showElements.grid )
            {
                this->update_grid( context );
            }
            this->update_selection( context );
        }
        ImGui::EndChild();

        this->update_informations( context );
    }

    void Selector::set_tileset( std::shared_ptr< tile::Set const > tileset )
    {
        m_tileset = tileset;
    }

    std::optional< tile::CellPos > Selector::get_tile_selected() const
    {
        return m_tileSelected;
    }

    void Selector::update_config( UpdateContext & /* context */ )
    {
        ImGui::Checkbox( "Enable Grid ?", &m_showElements.grid );
        ImGui::Checkbox( "Show Debug ?", &m_debugWindow.is_window_enabled() );
        if ( m_showElements.grid )
        {
            ImGui::ColorEdit4( "Grid Color", m_gridColor.to_table() );
        }
    }

    void Selector::update_scroll( UpdateContext & /* context */ )
    {
        m_subTileset.set_first_tile( ImGui::GetScroll() );
    }

    void Selector::update_image( UpdateContext & /* context */ )
    {
        m_subTileset.set_position( ImGui::GetCursorScreenPos() );
        ImGui::Image( m_tileset->get_texture() );
    }

    void Selector::update_grid( UpdateContext & /* context */ )
    {
        math::RectangleF gridRect { m_subTileset.get_rect() };
        gridRect += m_subTileset.first_tile().pixel().to_float();

        // Border
        ImGui::GetWindowDrawList()->AddRect( gridRect.pos, gridRect.bound_pos(),
                                             m_gridColor.to_integer() );

        // Horizontal lines
        for ( unsigned int x = 0; x < m_subTileset.get_size().tile().x; ++x )
        {
            float       xPixel = static_cast< float >( x ) * TILE_PIXEL_SIZE;
            math::LineF line { { gridRect.pos.x + xPixel, gridRect.pos.y },
                               { gridRect.pos.x + xPixel,
                                 gridRect.bound_pos().y } };

            ImGui::GetWindowDrawList()->AddLine( line.pointA, line.pointB,
                                                 m_gridColor.to_integer() );
        }
        // Vertical lines
        for ( unsigned int y = 0; y < m_subTileset.get_size().tile().y; ++y )
        {
            float       yPixel = static_cast< float >( y ) * TILE_PIXEL_SIZE;
            math::LineF line { { gridRect.pos.x, gridRect.pos.y + yPixel },
                               { gridRect.bound_pos().x,
                                 gridRect.pos.y + yPixel } };

            ImGui::GetWindowDrawList()->AddLine( line.pointA, line.pointB,
                                                 m_gridColor.to_integer() );
        }
    }

    void Selector::update_selection( UpdateContext & context )
    {
        if ( ! ImGui::IsWindowHovered() )
        {
            return;
        }

        math::Vector2I mousePos { context.inputs.get_mouse_position() };
        std::optional< math::Vector2F > rectPosOpt {
            m_subTileset.get_cell_abs_pos( mousePos.to_float() ) };
        if ( ! rectPosOpt.has_value() )
        {  // the mouse is not inside the tileset grid
            return;
        }

        math::Vector2F rectPosWithScroll { rectPosOpt.value() };

        // Selection rectangle
        ImGui::GetWindowDrawList()->AddRectFilled( rectPosWithScroll,
                                                   rectPosWithScroll
                                                       + TILE_PIXEL_SIZE_VECTOR,
                                                   m_gridColor.to_integer() );
        if ( context.inputs.is_pressed( sf::Mouse::Button::Left ) )
        {
            m_tileSelected = m_subTileset.get_cell_pos( mousePos.to_float() );
            if ( ! m_tileSelected.has_value() )
            {
                Trace::Error( "Tile should be selected" );
            }
        }
    }

    void Selector::update_informations( UpdateContext & /* context */ )
    {
        m_debugWindow.add_debug_text( "Tileset Position : {}",
                                      m_tileset->get_position() );
        m_debugWindow.add_debug_text( "Tileset Size : {}",
                                      m_tileset->get_size() );
        m_debugWindow.add_debug_text( "SubTileset Position : {}",
                                      m_subTileset.get_position() );
        m_debugWindow.add_debug_text( "SubTileset Size : {}",
                                      m_subTileset.get_size() );
        m_debugWindow.add_debug_text( "SubTileset First Tile : {}",
                                      m_subTileset.first_tile() );
        // TODO : overload fmt for std::optional
        m_debugWindow.add_debug_text(
            "Current Tile Selected : {}",
            m_tileSelected.has_value()
                ? std::to_string( m_tileSelected.value().index() )
                : "None" );
    }
}  // namespace tile
