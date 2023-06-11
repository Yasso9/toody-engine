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
#include "graphics2D/constants.hpp"     // for TILE_PIXEL_SIZE_I, TILE_PI...
#include "graphics2D/sfml.hpp"          // for operator<<
#include "graphics2D/tile/cellpos.hpp"  // for tile::Position, tile::Position...
#include "graphics2D/tile/set.hpp"      // for Tileset
#include "graphics2D/view.hpp"          // for View
#include "maths/geometry/point.hpp"     // for PointF
#include "maths/geometry/point.tpp"     // for Point::Point<Type>, Point:...
#include "tools/array/vector.hpp"
#include "tools/assertion.hpp"      // for ASSERTION
#include "tools/serialization.hpp"  // for Serializer, Unserializer
#include "tools/serialization.tpp"  // for Serializer::Serializer<Typ...
#include "tools/stream/stream.hpp"
#include "tools/tools.tpp"  // for is_rectangle

namespace tile
{
    Map::Map( std::shared_ptr< tile::Set const > tileset )
      : m_tileset { tileset },
        m_table { m_tileset->get_texture() },
        m_cursor { MouseCursor::Outline }
    {
        this->add_child( m_table );
        this->add_child( m_cursor );

        // m_cursor.on_click( [this] ( tile::CellPos const & tilePosition ) {
        //     Trace::Debug( "Clicked on tile: ", tilePosition.tile() );
        //     if ( m_tileSelector.get_tile_selected().has_value() )
        //     {
        //         this->change_tile( tilePosition,
        //                            m_tileSelector.get_tile_selected().value()
        //                            );
        //     }
        // } );

        this->load( Config::get_instance().get_tilemap_save_path() );
        // this->setPosition( 0.f, 0.f );
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

    math::Vector2F Map::get_position() const
    {
        return this->getPosition();
    }

    void Map::set_position( math::Vector2F const & pos )
    {
        this->setPosition( pos );
    }

    tile::Size Map::get_size() const
    {
        return m_table.get_size();
    }

    std::shared_ptr< tile::Set const > Map::get_tileset() const
    {
        return this->m_tileset;
    }

    void Map::load( std::filesystem::path const & path )
    {
        // Load table from file
        std::string content { fs::get_content( path ) };
        if ( content == "" )
        {
            content = "{ ( 1, 1 ) | [ (0, 0) ] }";
        }
        std::stringstream stream {};
        stream << content;
        stream >> m_table;
    }

    void Map::save() const
    {
        std::ofstream stream { Config::get_instance().get_tilemap_save_path(),
                               std::ios::out | std::ios::trunc };
        stream << m_table;
        stream.close();
    }

    void Map::change_tile( tile::CellPos position, tile::CellPos value )
    {
        if ( position >= this->get_size() )
        {
            Trace::Warning(
                "Tilemap position not comptatible with current tilemap" );
        }
        if ( value >= this->get_tileset()->get_size() )
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
            m_table.set_size( tile::Size { size, tile::Type::Tile } );
        }
    }

    void Map::update_informations( UpdateContext & /* context */ )
    {
        static bool showDebug { true };
        ImGui::Checkbox( "Show debug informations ?", &showDebug );
        if ( showDebug )
        {
            std::ostringstream infoOutput {};

            infoOutput << "Tilemap - Position : " << this->getPosition()
                       << "\n";
            infoOutput << "Tilemap - Size : " << this->get_size().pixel()
                       << "\n";
            infoOutput << "Tilemap - Number of Tile : "
                       << this->get_size().tile() << "\n";

            ImGui::Text( "%s", infoOutput.str().c_str() );
        }
    }
}  // namespace tile
