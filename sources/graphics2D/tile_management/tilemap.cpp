#include "tilemap.hpp"

#include <algorithm>  // for max
#include <cstdlib>    // for strtoul
#include <memory>     // for allocator_traits<>::value_...
#include <sstream>    // for operator<<, basic_ostream
#include <string>     // for char_traits, to_string

#include <IMGUI/imgui.h>                   // for Button, Text, InputText
#include <SFML/Graphics/Color.hpp>         // for Color, Color::Transparent
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/Graphics/VertexArray.hpp>   // for VertexArray
#include <SFML/Window/Mouse.hpp>           // for Mouse, Mouse::Button, Mous...
#include <ext/alloc_traits.h>              // for __alloc_traits<>::value_type

#include "component/component.hpp"  // for Component::add_child
#include "graphics2D/sfml.hpp"      // for operator<<
#include "graphics2D/tile_management/tile_position.hpp"  // for tile::Position, tile::Position...
#include "graphics2D/tile_management/tileset.hpp"  // for Tileset
#include "graphics2D/view.hpp"                     // for View
#include "input/input.hpp"            // for get_mouse_position, is_pre...
#include "main/render.hpp"            // for Render
#include "maths/geometry/point.hpp"   // for PointF
#include "maths/geometry/point.tpp"   // for Point::Point<Type>, Point:...
#include "tools/assertion.hpp"        // for ASSERTION
#include "tools/global_variable.hpp"  // for TILE_PIXEL_SIZE_I, TILE_PI...
#include "tools/serialization.hpp"    // for Serializer, Unserializer
#include "tools/serialization.tpp"    // for Serializer::Serializer<Typ...
#include "tools/tools.tpp"            // for is_rectangle
#include "tools/vector.hpp"

/// @todo create a button reset database who call
/// load_from_database()
/// @todo put a grid on the tilemap

TileMap::TileMap( View & view )
  : m_databaseTable { "tilemap" },
    m_tileSelector {},
    m_cursor {},
    m_view { view },
    m_tileTable {},
    m_currentDepth { 0u }
{
    this->add_child( m_tileSelector );

    this->load_from_database();

    this->setPosition( 0.f, 0.f );
}

void TileMap::update( float /* deltaTime */ )
{
    ImGui::SetNextWindowBgAlpha( 0.5f );
    if ( ImGui::Begin( "Tilemap Information" ) )
    {
        this->update_selection();

        this->update_table_informations();

        this->update_tile_size_button();

        if ( ImGui::Button( "Save Tilemap" ) )
        {
            this->save();
        }
    }
    ImGui::End();
}

void TileMap::render( Render & render ) const
{
    render.set_texture( this->get_tileset().get_texture() );

    for ( auto const & column : m_tileTable )
    {
        for ( auto const & tile : column )
        {
            for ( Tile const & cell : tile )
            {
                render.draw( cell.get_vertex_array() );
            }
        }
    }

    /// @todo Create a method cursor.draw
    render.draw( m_cursor.shape );
}

Tileset const & TileMap::get_tileset() const
{
    return this->m_tileSelector.get_tileset();
};

tile::Size TileMap::get_size() const
{
    return tile::Size {
        math::Vector2 {m_tileTable[0].size(), m_tileTable.size()}
         .to_u_int(),
        tile::Size::Tile
    };
}

math::Vector2F TileMap::get_center_absolute() const
{
    return this->get_center_relative() + math::Vector2F { this->getPosition() };
}

math::Vector2F TileMap::get_center_relative() const
{
    return math::Vector2F { this->get_size().pixel().to_float() / 2.f };
}

std::optional< tile::Position > TileMap::get_tile_position(
    math::PointF point ) const
{
    if ( ! this->contain( point ) )
    {
        return std::nullopt;
    }

    math::Vector2U pointRelative {
        point - math::Vector2F { this->getPosition() } };

    return tile::Position {
        pointRelative, this->get_size(), tile::Position::Pixel };
}

bool TileMap::contain( math::PointF point ) const
{
    if ( ! m_view.contain( point ) )
    {
        std::cerr << "The point isn't inside the view" << std::endl;
        return false;
    }

    return point.is_inside(
        math::PointF { this->getPosition() },
        this->get_size().pixel().to_float() );
}

void TileMap::resize( tile::Size tileSize )
{
    if ( this->get_size().tile() == tileSize.tile() )
    {
        // same size nothing to do
        return;
    }

    // resize the number of line
    m_tileTable.resize( tileSize.tile().y );

    // resize each column
    for ( unsigned int i_line { 0u }; i_line < tileSize.tile().y; ++i_line )
    {
        if ( m_tileTable[i_line].size() == tileSize.tile().x )
        {
            // same size nothing to do
            continue;
        }

        while ( m_tileTable[i_line].size() > tileSize.tile().x )
        {
            // size too big, must remove some elements
            m_tileTable[i_line].pop_back();
        }

        while ( m_tileTable[i_line].size() < tileSize.tile().x )
        {
            // size too low, must append some element
            // the default value must set the tile at the correct position
            Tile defaultValue { *this, m_tileSelector.get_tileset() };

            defaultValue.set_tileset_position( tile::Position {
                0u, m_tileSelector.get_tileset().get_size() } );

            defaultValue.set_tilemap_position( tile::Position {
                math::Vector2U {
                                static_cast< unsigned int >( m_tileTable[i_line].size() ),
                                i_line},
                this->get_size(), tile::Position::Tile
            } );

            m_tileTable[i_line].push_back( { defaultValue } );
        }
    }

    ASSERTION(
        vector::is_rectangle( m_tileTable ),
        "columns of the table must have the same size" );
}

void TileMap::save() const
{
    m_databaseTable.insert( database::TILEMAP, m_tileTable );
}

void TileMap::load_from_database()
{
    auto const table =
        m_databaseTable
            .select< std::vector< std::vector< std::vector< int > > > >(
                database::TILEMAP );

    tile::Position positionInTileset {
        0u, m_tileSelector.get_tileset().get_size().tile().x };
    tile::Position positionInTilemap {
        0u, static_cast< unsigned int >( table[0].size() ) };

    // Parse all the line of the tilemap
    for ( unsigned int line { 0u }; line < table.size(); ++line )
    {
        m_tileTable.push_back( {} );

        // Parse all the tile of each line of the tilemap (column)
        for ( unsigned int column { 0u }; column < table[line].size();
              ++column )
        {
            m_tileTable[line].push_back( {} );

            // Parse each cell of each tile of the tilemap
            for ( unsigned int depth { 0u }; depth < table[line][column].size();
                  ++depth )
            {
                Tile tile { *this, m_tileSelector.get_tileset() };

                /// @todo find a way to remove the cast
                positionInTileset.set_value(
                    static_cast< unsigned int >( table[line][column][depth] ) );
                positionInTilemap.set_value(
                    math::Vector2U { column, line }, tile::Position::Tile );

                tile.set_tilemap_position( positionInTilemap );
                tile.set_tileset_position( positionInTileset );

                // We add the element to the tile table
                m_tileTable[line][column].push_back( tile );
            }
        }
    }

    ASSERTION(
        vector::is_rectangle( table ) && vector::is_rectangle( m_tileTable ),
        "columns of the table haven't the same size" );
}

void TileMap::change_tile(
    tile::Position tilemapPosition, tile::Position tilesetPosition )
{
    if ( tilemapPosition >= this->get_size() )
    {
        std::cerr << "Tilemap position not comptatible with current tilemap"
                  << std::endl;
    }
    if ( tilesetPosition >= this->get_tileset().get_size() )
    {
        std::cerr << "Tileset position not comptatible with current tileset"
                  << std::endl;
    }

    Tile & currentTile {
        m_tileTable[tilemapPosition.tile().y][tilemapPosition.tile().x]
                   [m_currentDepth] };

    currentTile.set_tileset_position( tilesetPosition );
}

void TileMap::update_selection()
{
    static bool showDebug { true };
    ImGui::Checkbox( "Show debug informations ?", &showDebug );
    if ( showDebug )
    {
        std::stringstream infoOutput {};

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

        infoOutput << "Tilemap - Position : " << this->getPosition() << "\n";
        infoOutput << "Tilemap - Size : " << this->get_size().pixel() << "\n";
        infoOutput << "Tilemap - Number of Tile : " << this->get_size().tile()
                   << "\n";

        infoOutput << "View - Center : " << m_view.get_center() << "\n";
        infoOutput << "View - Size : " << m_view.get_size() << "\n";
        infoOutput << "View - Position : " << m_view.get_position() << "\n";
        infoOutput << "View - Zoom : " << m_view.get_zoom() << "\n";

        infoOutput << "Mouse Position - Absolute : "
                   << input::get_mouse_position() << "\n";
        infoOutput << "Mouse Position - Relativ to View : "
                   << input::get_mouse_position_relative( m_view ) << "\n";

        ImGui::Text( "%s", infoOutput.str().c_str() );
    }

    math::PointF mousePosition {
        input::get_mouse_position_relative( m_view ).to_point() };

    if ( ! this->get_tile_position( mousePosition ).has_value()
         || ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) )
    {
        // The mouse is outside the tilemap
        m_cursor.shape.setOutlineColor( sf::Color::Transparent );
        return;
    }

    tile::Position tilePosition {
        this->get_tile_position( mousePosition ).value() };

    if ( input::is_pressed( sf::Mouse::Button::Left )
         && m_tileSelector.get_tile_selected().has_value() )
    {
        // There's a left click and the mouse is inside the tilemap
        this->change_tile(
            tilePosition, m_tileSelector.get_tile_selected().value() );
    }

    // The mouse is inside the tilemap, we show the cursor
    m_cursor.shape.setOutlineColor( sf::Color { 48, 48, 48, 128 } );
    m_cursor.shape.setPosition( tilePosition.pixel().to_float() );
}

void TileMap::update_table_informations()
{
    static std::stringstream tileValueStream {};

    if ( ImGui::BeginTable(
             "table_padding",
             static_cast< int >( this->get_size().tile().x ) ) )
    {
        for ( unsigned int line { 0u }; line < this->get_size().tile().y;
              ++line )
        {
            ImGui::TableNextRow();
            for ( unsigned int column { 0u };
                  column < this->get_size().tile().x; ++column )
            {
                ImGui::TableSetColumnIndex( static_cast< int >( column ) );

                std::stringstream buttonStream {};
                buttonStream << math::Vector2U { column, line } << " -> "
                             << m_tileTable[line][column][m_currentDepth]
                                    .get_tileset_position()
                                    .value();

                if ( ImGui::Button( buttonStream.str().c_str() ) )
                {
                    tileValueStream.str( "" );

                    for ( Tile const & element : m_tileTable[line][column] )
                    {
                        tileValueStream << element.get_debug_info() << "\n";
                    }
                }
            }
        }
        ImGui::EndTable();
    }

    if ( ImGui::Button( "Clear Tile Information" ) )
    {
        tileValueStream.str( "" );
    }

    ImGui::Text( "%s", tileValueStream.str().c_str() );
}

void TileMap::update_tile_size_button()
{
    std::array< int, 2 > tileNumbers {
        this->get_size().tile().to_int().x,
        this->get_size().tile().to_int().y };

    std::string newSize { "" };

    if ( tileNumbers[0] != this->get_size().tile().to_int().x
         || tileNumbers[1] != this->get_size().tile().to_int().y )
    {
        std::stringstream stream {};
        stream << "Current tilemap size : " << this->getPosition() << "\n";
        ImGui::Text( "%s", stream.str().c_str() );

        newSize = "New ";
    }

    ImGui::InputInt2(
        "Tilemap Size :", &tileNumbers[0], ImGuiInputTextFlags_CharsDecimal );

    if ( ImGui::Button( "Update Size" ) )
    {
        this->resize( tile::Size {
            math::Vector2I {tileNumbers[0], tileNumbers[1]}
             .to_u_int(),
            tile::Size::Tile
        } );

        ASSERTION(
            tileNumbers[0] == this->get_size().tile().to_int().x
                && tileNumbers[1] == this->get_size().tile().to_int().y,
            "Issue with tilemap resize" );

        newSize = "";
    }
}