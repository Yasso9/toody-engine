#include "map.hpp"

#include <algorithm>  // for max
#include <cstdlib>    // for strtoul
#include <fstream>
#include <memory>   // for allocator_traits<>::value_...
#include <sstream>  // for operator<<, basic_ostream
#include <string>   // for char_traits, to_string

#include <SFML/Graphics/Color.hpp>         // for Color, Color::Transparent
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/Graphics/VertexArray.hpp>   // for VertexArray
#include <SFML/Window/Mouse.hpp>           // for Mouse, Mouse::Button, Mous...
#include <ext/alloc_traits.h>              // for __alloc_traits<>::value_type
#include <imgui/imgui.h>                   // for Button, Text, InputText

#include "application/components/component.hpp"  // for Component::add_child
#include "application/configuration.hpp"         // for Config
#include "application/resources.hpp"             // for resource::app_data
#include "graphics2D/constants.hpp"      // for TILE_PIXEL_SIZE_I, TILE_PI...
#include "graphics2D/sfml.hpp"           // for operator<<
#include "graphics2D/tile/position.hpp"  // for tile::Position, tile::Position...
#include "graphics2D/tile/set.hpp"       // for Tileset
#include "graphics2D/view.hpp"           // for View
#include "maths/geometry/point.hpp"      // for PointF
#include "maths/geometry/point.tpp"      // for Point::Point<Type>, Point:...
#include "tools/array/vector.hpp"
#include "tools/assertion.hpp"      // for ASSERTION
#include "tools/serialization.hpp"  // for Serializer, Unserializer
#include "tools/serialization.tpp"  // for Serializer::Serializer<Typ...
#include "tools/stream/stream.hpp"
#include "tools/tools.tpp"  // for is_rectangle

namespace tile
{
    Map::Map( View & view )
      : m_tileSelector {},
        m_table { m_tileSelector.get_tileset().get_texture() },
        m_cursor { MouseCursor::Outline },
        m_view { view }
    {
        this->add_child( m_tileSelector );
        this->add_child( m_table );
        this->add_child( m_cursor );

        m_cursor.on_click( [this] ( tile::Position const & tilePosition ) {
            Trace::Debug( "Clicked on tile: ", tilePosition.tile() );
            if ( m_tileSelector.get_tile_selected().has_value() )
            {
                this->change_tile( tilePosition,
                                   m_tileSelector.get_tile_selected().value() );
            }
        } );

        // Load table from file
        std::string content {
            fs::get_content( Config::get_instance().get_tilemap_save_path() ) };
        if ( content == "" )
        {
            content = "{ ( 1, 1 ) | [ (0, 0) ] }";
        }
        std::stringstream stream {};
        stream << content;
        stream >> m_table;

        this->setPosition( 0.f, 0.f );
    }

    void Map::update( UpdateContext & context )
    {
        m_cursor.manual_update( context, *this );

        if ( ImGui::BeginWindow( "Tilemap Informations" ) )
        {
            this->update_size( context );

            this->update_informations( context );

            if ( ImGui::Button( "Save Tilemap" ) )
            {
                this->save();
            }
        }
        ImGui::EndWindow();
    }

    tile::Set const & Map::get_tileset() const
    {
        return this->m_tileSelector.get_tileset();
    }

    tile::Size Map::get_size() const
    {
        return m_table.get_size();
    }

    math::Vector2F Map::get_center_absolute() const
    {
        return this->get_center_relative()
               + math::Vector2F { this->getPosition() };
    }

    math::Vector2F Map::get_center_relative() const
    {
        return math::Vector2F { this->get_size().pixel().to_float() / 2.f };
    }

    View const & Map::get_view() const
    {
        return m_view;
    }

    std::optional< tile::Position > Map::get_position(
        math::PointF point ) const
    {
        if ( ! this->contain( point ) )
        {
            return std::nullopt;
        }

        math::Vector2U pointRelative {
            point - math::Vector2F { this->getPosition() } };

        return tile::Position { pointRelative, this->get_size(),
                                tile::Position::Pixel };
    }

    bool Map::contain( math::PointF point ) const
    {
        if ( ! m_view.contain( point ) )
        {
            return false;
        }

        return point.is_inside( math::PointF { this->getPosition() },
                                this->get_size().pixel().to_float() );
    }

    bool Map::is_comptatible( tile::Position position ) const
    {
        return this->get_size().tile().x == position.get_number_of_columns();
    }

    void Map::save() const
    {
        std::ofstream stream { Config::get_instance().get_tilemap_save_path(),
                               std::ios::out | std::ios::trunc };
        stream << m_table;
        stream.close();
    }

    void Map::change_tile( tile::Position position, tile::Position value )
    {
        if ( position >= this->get_size() )
        {
            Trace::Warning(
                "Tilemap position not comptatible with current tilemap" );
        }
        if ( value >= this->get_tileset().get_size() )
        {
            Trace::Warning(
                "Tileset position not comptatible with current "
                "tileset" );
        }

        m_table( position.tile().x, position.tile().y ).set_value( value );
    }

    void Map::update_size( UpdateContext & /* context */ )
    {
        math::Vector2U size { this->get_size().tile() };
        if ( ImGui::SliderVec( "Tilemap Size", size, 1, 50 ) )
        {
            m_table.set_size( tile::Size { size, tile::Size::Tile } );
        }
    }

    void Map::update_informations( UpdateContext & context )
    {
        static bool showDebug { true };
        ImGui::Checkbox( "Show debug informations ?", &showDebug );
        if ( showDebug )
        {
            std::ostringstream infoOutput {};

            if ( m_tileSelector.get_tile_selected().has_value() )
            {
                infoOutput << "Tileset - Tile Selected : "
                           << m_tileSelector.get_tile_selected().value().value()
                           << "\n";
            }
            else
            {
                infoOutput << "Tileset - Tile Selected : None\n";
            }

            infoOutput << "Tilemap - Position : " << this->getPosition()
                       << "\n";
            infoOutput << "Tilemap - Size : " << this->get_size().pixel()
                       << "\n";
            infoOutput << "Tilemap - Number of Tile : "
                       << this->get_size().tile() << "\n";

            infoOutput << "View - Center : " << m_view.get_center() << "\n";
            infoOutput << "View - Size : " << m_view.get_size() << "\n";
            infoOutput << "View - Position : " << m_view.get_position() << "\n";
            infoOutput << "View - Zoom : " << m_view.get_zoom( context.window )
                       << "\n";

            ImGui::Text( "%s", infoOutput.str().c_str() );
        }
    }
}  // namespace tile
