#include "tilemap.hpp"

#include "libraries/imgui.hpp"
#include "main/window.hpp"
#include "tools/databases.hpp"
#include "tools/global_variable.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

/// @todo create a button reset database who call init_tile_table_from_database()
/// @todo put a grid on the tilemap

TileMap::TileMap( View & view )
  : m_tileSelector(),
    m_cursor(),
    m_view( view ),
    m_tileTable(),
    m_currentDepth( 0u ),
    m_isLeftButtonPressed( false ),
    m_mousePosition( 0.f, 0.f )
{
    this->init_tile_table_from_database();

    // Tilemap Cursor Initialisation
    m_cursor.setSize( TILE_PIXEL_SIZE_VECTOR );
    m_cursor.setOutlineThickness( -3.f );
    m_cursor.setOutlineColor( sf::Color::Transparent );
    m_cursor.setFillColor( sf::Color::Transparent );
    m_cursor.setPosition( 0.f, 0.f );

    this->setPosition( 0.f, 0.f );
}

math::Vector2F TileMap::get_size() const
{
    return this->get_tile_size().to_float() * TILE_PIXEL_SIZE_VECTOR;
}

math::Vector2U TileMap::get_tile_size() const
{
    /// @todo faire un constructeur de size_t à partir de vector2U seulement
    return math::Vector2U {
        static_cast< unsigned int >( this->m_tileTable[0].size() ),
        static_cast< unsigned int >( this->m_tileTable.size() )
    };
}

void TileMap::set_tile_size( math::Vector2U const & tileSize )
{
    if ( this->get_tile_size() == tileSize )
    {
        // same size nothing to do
        return;
    }

    // resize the number of line
    this->m_tileTable.resize( tileSize.y );

    // resize each column
    for ( unsigned int i_line { 0u }; i_line < tileSize.y; ++i_line )
    {
        if ( this->m_tileTable[i_line].size() == tileSize.x )
        {
            // same size nothing to do
            continue;
        }

        while ( this->m_tileTable[i_line].size() > tileSize.x )
        {
            // size too big, must remove some elements
            this->m_tileTable[i_line].pop_back();
        }

        while ( this->m_tileTable[i_line].size() < tileSize.x )
        {
            // size too low, must append some element
            // the defualt value must set the tile at the correct position
            Tile defaultValue { *this, this->m_tileSelector };
            defaultValue.set_positions(
                TilePosition {
                    0u,
                    this->m_tileSelector.get_tileset().get_size_in_tile().x
            },
                TilePosition {
                    math::Vector2U { static_cast< unsigned int >(
                                         this->m_tileTable[i_line].size() ),
                                     i_line },
                    this->get_tile_size().x,
                    TilePosition::Tile } );

            this->m_tileTable[i_line].push_back( { defaultValue } );
        }
    }

    ASSERTION( tools::is_rectangle( this->m_tileTable ),
               "columns of the table haven't the same size" );
}

void TileMap::process_events()
{
    this->m_mousePosition = sf::Mouse::getPosition( Window::get_instance() );
    this->m_tileSelector.process_events();

    if ( ImGui::P_IsAnyWindowFocused() )
    {
        this->m_isLeftButtonPressed = false;
        return;
    }

    this->m_isLeftButtonPressed =
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left );
}

void TileMap::update()
{
    this->m_tileSelector.update();

    ImGui::SetNextWindowBgAlpha( 0.5f );
    if ( ImGui::P_Begin( "Tilemap Information" ) )
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
    db::request( "INSERT INTO tilemap (tile_table)"
                 "VALUES('"
                 + Serializer { this->m_tileTable }.to_string() + "');" );
}

static unsigned int tileset_value( int tileValue )
{
    return tileValue >= 0 ? static_cast< unsigned int >( tileValue ) : 0u;
}

void TileMap::init_tile_table_from_database()
{
    auto const table =
        db::request( "SELECT tile_table FROM tilemap;" )
            .to_value< std::vector< std::vector< std::vector< int > > > >();

    TilePosition positionInTileset {
        0u,
        this->m_tileSelector.get_tileset().get_size_in_tile().x
    };
    TilePosition positionInTilemap { 0u,
                                     static_cast< unsigned int >(
                                         table[0].size() ) };

    // Parse all the line of the tilemap
    for ( unsigned int line { 0u }; line < table.size(); ++line )
    {
        this->m_tileTable.push_back( {} );

        // Parse all the tile of each line of the tilemap (column)
        for ( unsigned int column { 0u }; column < table[line].size();
              ++column )
        {
            this->m_tileTable[line].push_back( {} );

            // Parse each cell of each tile of the tilemap
            for ( unsigned int depth { 0u }; depth < table[line][column].size();
                  ++depth )
            {
                Tile tile { *this, this->m_tileSelector };

                positionInTileset.set_value(
                    tileset_value( table[line][column][depth] ) );
                positionInTilemap.set_value( math::Vector2U { column, line },
                                             TilePosition::Tile );

                tile.set_positions( positionInTileset, positionInTilemap );

                // We add the element to the tile table
                this->m_tileTable[line][column].push_back( tile );
            }
        }
    }

    ASSERTION( tools::is_rectangle( table )
                   && tools::is_rectangle( this->m_tileTable ),
               "columns of the table haven't the same size" );
}

void TileMap::change_tile( math::Vector2U const & tilePositionInTile,
                           int const & newTileValue )
{
    ASSERTION( tilePositionInTile < this->get_tile_size(), "position too big" );
    ASSERTION( newTileValue < static_cast< int >(
                   this->get_tileset().get_number_of_tile() ),
               "Tileset value too big" );

    Tile & currentTile {
        this->m_tileTable[tilePositionInTile.y][tilePositionInTile.x]
                         [this->m_currentDepth]
    };

    currentTile.set_position_in_tileset(
        TilePosition { tileset_value( newTileValue ),
                       this->get_tileset().get_size_in_tile().x } );
}

void TileMap::update_selection()
{
    std::stringstream infoOutput {};

    infoOutput << "Tileset - Tile Selected : "
               << this->m_tileSelector.get_tile_selected() << "\n";

    infoOutput << "Tilemap - Position : " << this->getPosition() << "\n";
    infoOutput << "Tilemap - Size : " << this->get_size() << "\n";
    /// @todo create a number of tile method
    infoOutput << "Tilemap - Number of Tile : "
               << this->get_size() / TILE_PIXEL_SIZE << "\n";

    infoOutput << "View - Center : " << this->m_view.get_center() << "\n";
    infoOutput << "View - Size : " << this->m_view.get_size() << "\n";
    infoOutput << "View - Position : " << this->m_view.get_position() << "\n";
    infoOutput << "View - Zoom : " << this->m_view.get_zoom() << "\n";

    math::Vector2F const mousePositionViewZoom {
        this->m_mousePosition.to_float() / this->m_view.get_zoom()
    };
    math::Vector2F const mousePositionRelativToView { math::floor(
        mousePositionViewZoom + this->m_view.get_position() ) };
    infoOutput << "Mouse Position - Absolute : " << this->m_mousePosition
               << "\n";
    infoOutput << "Mouse Position - View Zoom : " << mousePositionViewZoom
               << "\n";
    infoOutput << "Mouse Position - Relativ to View : "
               << mousePositionRelativToView << "\n";

    ImGui::Text( "%s", infoOutput.str().c_str() );

    if ( ImGui::P_IsAnyWindowHovered()
         || ! mousePositionRelativToView.is_inside( this->getPosition(),
                                                    this->get_size() ) )
    {
        // The mouse is outside the tilemap
        this->m_cursor.setOutlineColor( sf::Color::Transparent );
        return;
    }

    std::stringstream selectionOutput {};

    math::Vector2F const mousePositionRelativToTilemap {
        mousePositionRelativToView - math::Vector2F { this->getPosition() }
    };
    selectionOutput << "Mouse Position - Relativ to Tilemap : "
                    << mousePositionRelativToTilemap << "\n";

    math::Vector2I const tileSelectedPositionInPixel {
        mousePositionRelativToTilemap.to_int()
        - ( mousePositionRelativToTilemap.to_int() % TILE_PIXEL_SIZE_I )
    };
    selectionOutput << "Tile Position - in Pixel : "
                    << tileSelectedPositionInPixel << "\n";

    math::Vector2U const tileSelectedPositionInTile {
        tileSelectedPositionInPixel / TILE_PIXEL_SIZE_I
    };
    selectionOutput << "Tile Position - in Tile : "
                    << tileSelectedPositionInTile << "\n";

    ImGui::Text( "%s", selectionOutput.str().c_str() );

    if ( this->m_isLeftButtonPressed )
    {
        // There's a left click and the mouse is inside the tilemap
        this->change_tile( tileSelectedPositionInTile,
                           this->m_tileSelector.get_tile_selected() );
    }

    // The mouse is inside the tilemap, we show the cursor
    this->m_cursor.setOutlineColor( sf::Color::Black );
    this->m_cursor.setPosition( tileSelectedPositionInPixel.to_float() );
}

void TileMap::update_table_informations()
{
    static std::stringstream tileValueStream {};

    if ( ImGui::BeginTable( "table_padding",
                            static_cast< int >( this->get_tile_size().x ) ) )
    {
        for ( unsigned int line { 0u }; line < this->get_tile_size().y; ++line )
        {
            ImGui::TableNextRow();
            for ( unsigned int column { 0u }; column < this->get_tile_size().x;
                  ++column )
            {
                ImGui::TableSetColumnIndex( static_cast< int >( column ) );

                std::stringstream buttonStream {};
                buttonStream
                    << math::Vector2U { column, line } << " -> "
                    << this->m_tileTable[line][column][this->m_currentDepth]
                           .get_position_in_tileset()
                           .value();

                if ( ImGui::Button( buttonStream.str().c_str() ) )
                {
                    tileValueStream.str( "" );

                    for ( Tile const & element :
                          this->m_tileTable[line][column] )
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

    static std::string tileNumberX { std::to_string(
        this->get_tile_size().x ) };
    static std::string tileNumberY { std::to_string(
        this->get_tile_size().y ) };

    ImGui::InputText( "Number of Tile X :",
                      tileNumberX.data(),
                      MAX_STRING_SIZE,
                      ImGuiInputTextFlags_CharsDecimal );
    ImGui::InputText( "Number of Tile Y :",
                      tileNumberY.data(),
                      MAX_STRING_SIZE,
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

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    states.texture = &this->m_tileSelector.get_tileset().get_texture();

    for ( auto const & column : this->m_tileTable )
    {
        for ( auto const & tile : column )
        {
            for ( Tile const & cell : tile )
            {
                target.draw( cell.get_vertex_array(), states );
            }
        }
    }

    target.draw( this->m_cursor, states );
}