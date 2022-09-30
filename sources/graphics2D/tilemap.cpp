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

#include "graphics2D/component.tpp"      // for Component::add_child
#include "graphics2D/sfml.hpp"           // for operator<<
#include "graphics2D/tile_position.hpp"  // for TilePosition, TilePosition...
#include "graphics2D/tileset.hpp"        // for Tileset
#include "graphics2D/view.hpp"           // for View
#include "input/input.hpp"               // for get_mouse_position, is_pre...
#include "main/render.hpp"               // for Render
#include "maths/geometry/point.hpp"      // for PointF
#include "maths/geometry/point.tpp"      // for Point::Point<Type>, Point:...
#include "tools/assertion.hpp"           // for ASSERTION
#include "tools/global_variable.hpp"     // for TILE_PIXEL_SIZE_I, TILE_PI...
#include "tools/serialization.hpp"       // for Serializer, Unserializer
#include "tools/serialization.tpp"       // for Serializer::Serializer<Typ...
#include "tools/tools.tpp"               // for is_rectangle
#include "tools/vector.hpp"

/// @todo create a button reset database who call
/// init_tile_table_from_database()
/// @todo put a grid on the tilemap

TileMap::TileMap( View & view )
  : m_databaseTable { "tilemap" },
    m_tileSelector {},
    m_cursor {},
    m_view { view },
    m_tileTable {},
    m_currentDepth { 0u }
{
    this->init_tile_table_from_database();

    // Tilemap Cursor Initialisation
    m_cursor.setSize( TILE_PIXEL_SIZE_VECTOR );
    m_cursor.setOutlineThickness( -3.f );
    m_cursor.setOutlineColor( sf::Color::Transparent );
    m_cursor.setFillColor( sf::Color::Transparent );
    m_cursor.setPosition( 0.f, 0.f );

    this->setPosition( 0.f, 0.f );

    this->add_child( m_tileSelector );
}

math::Vector2F TileMap::get_size() const
{
    return this->get_tile_size().to_float() * TILE_PIXEL_SIZE_VECTOR;
}

math::Vector2U TileMap::get_tile_size() const
{
    return math::Vector2U {
        static_cast< unsigned int >( m_tileTable[0].size() ),
        static_cast< unsigned int >( m_tileTable.size() ) };
}

void TileMap::set_tile_size( math::Vector2U const & tileSize )
{
    if ( this->get_tile_size() == tileSize )
    {
        // same size nothing to do
        return;
    }

    // resize the number of line
    m_tileTable.resize( tileSize.y );

    // resize each column
    for ( unsigned int i_line { 0u }; i_line < tileSize.y; ++i_line )
    {
        if ( m_tileTable[i_line].size() == tileSize.x )
        {
            // same size nothing to do
            continue;
        }

        while ( m_tileTable[i_line].size() > tileSize.x )
        {
            // size too big, must remove some elements
            m_tileTable[i_line].pop_back();
        }

        while ( m_tileTable[i_line].size() < tileSize.x )
        {
            // size too low, must append some element
            // the default value must set the tile at the correct position
            Tile defaultValue { *this, m_tileSelector };
            defaultValue.set_positions(
                TilePosition {
                    0u, m_tileSelector.get_tileset().get_size_in_tile().x
            },
                TilePosition {
                    math::Vector2U {
                        static_cast< unsigned int >(
                            m_tileTable[i_line].size() ),
                        i_line },
                    this->get_tile_size().x, TilePosition::Tile } );

            m_tileTable[i_line].push_back( { defaultValue } );
        }
    }

    ASSERTION(
        vector::is_rectangle( m_tileTable ),
        "columns of the table must have the same size" );
}

void TileMap::update_before( float /* deltaTime */ )
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

void TileMap::save() const
{
    m_databaseTable.insert( "tile_table", m_tileTable );
}

void TileMap::init_tile_table_from_database()
{
    auto const table =
        m_databaseTable
            .select< std::vector< std::vector< std::vector< int > > > >(
                "tile_table" );

    TilePosition positionInTileset {
        0u, m_tileSelector.get_tileset().get_size_in_tile().x };
    TilePosition positionInTilemap {
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
                Tile tile { *this, m_tileSelector };

                /// @todo correct this
                // positionInTileset.set_value( table[line][column][depth] );
                positionInTilemap.set_value(
                    math::Vector2U { column, line }, TilePosition::Tile );

                tile.set_positions( positionInTileset, positionInTilemap );

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
    math::Vector2U const & tilePositionInTile,
    unsigned int const &   newTileValue )
{
    ASSERTION( tilePositionInTile < this->get_tile_size(), "position too big" );
    ASSERTION(
        newTileValue < this->get_tileset().get_number_of_tile(),
        "Tileset value too big" );

    Tile & currentTile { m_tileTable[tilePositionInTile.y][tilePositionInTile.x]
                                    [m_currentDepth] };

    currentTile.set_position_in_tileset( TilePosition {
        newTileValue, this->get_tileset().get_size_in_tile().x } );
}

void TileMap::update_selection()
{
    std::stringstream infoOutput {};

    infoOutput << "Tileset - Tile Selected : "
               << m_tileSelector.get_tile_selected().value() << "\n";

    infoOutput << "Tilemap - Position : " << this->getPosition() << "\n";
    infoOutput << "Tilemap - Size : " << this->get_size() << "\n";
    /// @todo create a number of tile method
    infoOutput << "Tilemap - Number of Tile A : "
               << this->get_size() / TILE_PIXEL_SIZE << "\n";
    infoOutput << "Tilemap - Number of Tile B (if same than A, remove A) : "
               << this->get_tile_size() << "\n";

    infoOutput << "View - Center : " << m_view.get_center() << "\n";
    infoOutput << "View - Size : " << m_view.get_size() << "\n";
    infoOutput << "View - Position : " << m_view.get_position() << "\n";
    infoOutput << "View - Zoom : " << m_view.get_zoom() << "\n";

    math::Vector2F const mousePositionViewZoom {
        input::get_mouse_position().to_float() / m_view.get_zoom() };
    math::PointF const mousePositionRelativToView {
        math::floor( mousePositionViewZoom + m_view.get_position() ) };
    infoOutput << "Mouse Position - Absolute : " << input::get_mouse_position()
               << "\n";
    infoOutput << "Mouse Position - View Zoom : " << mousePositionViewZoom
               << "\n";
    infoOutput << "Mouse Position - Relativ to View : "
               << mousePositionRelativToView << "\n";

    ImGui::Text( "%s", infoOutput.str().c_str() );

    if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow )
         || ! mousePositionRelativToView.is_inside(
             math::PointF { this->getPosition() }, this->get_size() ) )
    {
        // The mouse is outside the tilemap
        m_cursor.setOutlineColor( sf::Color::Transparent );
        return;
    }

    std::stringstream selectionOutput {};

    math::Vector2F const mousePositionRelativToTilemap {
        mousePositionRelativToView - math::Vector2F { this->getPosition() } };
    selectionOutput << "Mouse Position - Relativ to Tilemap : "
                    << mousePositionRelativToTilemap << "\n";

    math::Vector2I const tileSelectedPositionInPixel {
        mousePositionRelativToTilemap.to_int()
        - ( mousePositionRelativToTilemap.to_int() % TILE_PIXEL_SIZE_I ) };
    selectionOutput << "Tile Position - in Pixel : "
                    << tileSelectedPositionInPixel << "\n";

    math::Vector2U const tileSelectedPositionInTile {
        tileSelectedPositionInPixel / TILE_PIXEL_SIZE_I };
    selectionOutput << "Tile Position - in Tile : "
                    << tileSelectedPositionInTile << "\n";

    ImGui::Text( "%s", selectionOutput.str().c_str() );

    if ( input::is_pressed( sf::Mouse::Button::Left ) )
    {
        // There's a left click and the mouse is inside the tilemap
        this->change_tile(
            tileSelectedPositionInTile,
            m_tileSelector.get_tile_selected().value() );
    }

    // The mouse is inside the tilemap, we show the cursor
    m_cursor.setOutlineColor( sf::Color::Black );
    m_cursor.setPosition( tileSelectedPositionInPixel.to_float() );
}

void TileMap::update_table_informations()
{
    static std::stringstream tileValueStream {};

    if ( ImGui::BeginTable(
             "table_padding", static_cast< int >( this->get_tile_size().x ) ) )
    {
        for ( unsigned int line { 0u }; line < this->get_tile_size().y; ++line )
        {
            ImGui::TableNextRow();
            for ( unsigned int column { 0u }; column < this->get_tile_size().x;
                  ++column )
            {
                ImGui::TableSetColumnIndex( static_cast< int >( column ) );

                std::stringstream buttonStream {};
                buttonStream << math::Vector2U { column, line } << " -> "
                             << m_tileTable[line][column][m_currentDepth]
                                    .get_position_in_tileset()
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
    constexpr unsigned int const MAX_STRING_SIZE { 10u };

    static std::string tileNumberX {
        std::to_string( this->get_tile_size().x ) };
    static std::string tileNumberY {
        std::to_string( this->get_tile_size().y ) };

    ImGui::InputText(
        "Number of Tile X :", tileNumberX.data(), MAX_STRING_SIZE,
        ImGuiInputTextFlags_CharsDecimal );
    ImGui::InputText(
        "Number of Tile Y :", tileNumberY.data(), MAX_STRING_SIZE,
        ImGuiInputTextFlags_CharsDecimal );

    if ( ImGui::Button( "Update Size" ) )
    {
        /// @todo useful to have this pointers ?
        char * tileNumberXEnd;
        char * tileNumberYEnd;
        this->set_tile_size( math::Vector2U {
            std::strtoul( tileNumberX.c_str(), &tileNumberXEnd, 10 ),
            std::strtoul( tileNumberY.c_str(), &tileNumberYEnd, 10 ) } );
    }
}

void TileMap::render_before( Render & render ) const
{
    render.set_texture( m_tileSelector.get_tileset().get_texture() );

    for ( auto const & column : m_tileTable )
    {
        for ( auto const & tile : column )
        {
            for ( Tile const & cell : tile )
            {
                render.get_target().draw(
                    cell.get_vertex_array(), render.get_state() );
            }
        }
    }

    render.get_target().draw( m_cursor, render.get_state() );
}