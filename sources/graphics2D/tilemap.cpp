#include "tilemap.hpp"

#include "libraries/imgui.hpp"
#include "main/window.hpp"
#include "tools/databases.hpp"
#include "tools/global_variable.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

TileMap::TileMap( sf::View & view )
  : m_tileSelector(),
    m_cursor(),
    m_view( view ),
    m_tileTable(),
    m_currentDepth( 0u ),
    m_isLeftButtonPressed( false ),
    m_mousePosition( 0.f, 0.f )
{
    this->set_tile_table(
        db::request( "SELECT tile_table FROM tilemap;" )
            .to_value< std::vector< std::vector< std::vector< int > > > >() );

    m_cursor.setSize( TILE_PIXEL_SIZE_VECTOR );
    m_cursor.setOutlineThickness( -3.f );
    m_cursor.setOutlineColor( sf::Color::Transparent );
    m_cursor.setFillColor( sf::Color::Transparent );
    m_cursor.setPosition( 0.f, 0.f );

    this->setPosition( 0.f, 0.f );
}

math::Vector2F TileMap::get_size() const
{
    /// @todo il faut verifier que toute les ligne de toutes les colonnes fasse la même taille
    return this->get_tile_size() * TILE_PIXEL_SIZE_VECTOR;
}

math::Vector2S TileMap::get_tile_size() const
{
    /// @todo assert that the size of all element of this->m_tileTable have the same size
    return math::Vector2S { this->m_tileTable[0].size(),
                            this->m_tileTable.size() };
}

void TileMap::set_tile_size( math::Vector2S const & tileSize )
{
    if ( this->get_tile_size() == tileSize )
    {
        return;
    }

    // resize the number of line
    this->m_tileTable.resize( tileSize.y );

    // resize each column
    for ( unsigned int line { 0u }; line < tileSize.y; ++line )
    {
        if ( this->m_tileTable[line].size() == tileSize.x )
        {
            continue;
        }

        while ( this->m_tileTable[line].size() > tileSize.x )
        {
            this->m_tileTable[line].pop_back();
        }

        while ( this->m_tileTable[line].size() < tileSize.x )
        {
            Tile defaultValue { *this, this->m_tileSelector };
            defaultValue.set_data(
                0,
                math::Vector2U { static_cast< unsigned int >(
                                     this->m_tileTable[line].size() ),
                                 line } );

            this->m_tileTable[line].push_back( { defaultValue } );
        }
    }
}

void TileMap::process_events()
{
    m_isLeftButtonPressed =
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left );
    m_mousePosition = sf::Mouse::getPosition( Window::get_instance() );

    this->m_tileSelector.process_events();
}

void TileMap::update()
{
    this->m_tileSelector.update();

    if ( ImGui::P_Begin( "Tilemap Information" ) )
    {
        this->update_informations();

        this->update_table_informations();

        this->update_tile_size_button();

        if ( ImGui::Button( "Save Tilemap" ) )
        {
            this->save();
        }
        ImGui::End();
    }
}

void TileMap::save() const
{
    std::vector< std::vector< std::vector< int > > > valueArray {};
    for ( unsigned int line { 0u }; line < this->m_tileTable.size(); ++line )
    {
        valueArray.push_back( {} );
        for ( unsigned int column { 0u };
              column < this->m_tileTable[line].size();
              ++column )
        {
            valueArray[line].push_back( {} );
            for ( unsigned int depth { 0u };
                  depth < this->m_tileTable[line][column].size();
                  ++depth )
            {
                valueArray[line][column].push_back(
                    this->m_tileTable[line][column][depth].get_value() );
            }
        }
    }

    db::request( "INSERT INTO tilemap (tile_table)"
                 "VALUES('"
                 + Serializer { valueArray }.to_string() + "');" );
}

void TileMap::set_tile_table(
    std::vector< std::vector< std::vector< int > > > const & table )
{
    // Parse all the line of the tilemap
    for ( unsigned int line { 0u }; line < table.size(); ++line )
    {
        this->m_tileTable.push_back( {} );

        // Parse all the column of the tilemap
        for ( unsigned int column { 0u }; column < table[line].size();
              ++column )
        {
            this->m_tileTable[line].push_back( {} );

            // Parse each cell of each tile of the tilemap
            for ( unsigned int depth { 0u }; depth < table[line][column].size();
                  ++depth )
            {
                Tile tile { *this, this->m_tileSelector };
                tile.set_data( table[line][column][depth],
                               math::Vector2U { column, line } );

                // We add the element to the tile table
                this->m_tileTable[line][column].push_back( tile );
            }
        }
    }
}

void TileMap::change_tile( math::Vector2U const & tilePositionInTile,
                           int const & newTileValue )
{
    ASSERTION( tilePositionInTile < this->get_tile_size(), "position too big" );
    /// @todo assert that newTileValue is between 0 and max number of tile (maybe put it unsigned int idk)

    this->m_tileTable[tilePositionInTile.y][tilePositionInTile.x]
                     [this->m_currentDepth]
                         .set_value( newTileValue );
}

void TileMap::update_informations()
{
    math::Vector2I const mousePosition { this->m_mousePosition };
    math::Vector2F const viewZoom { Window::get_instance().get_size_f()
                                    / this->m_view.getSize() };
    math::Vector2F mouseViewPosition {
        ( mousePosition / viewZoom )
        - math::Vector2F { this->m_view.getSize() / 2.f }
        + math::Vector2F { this->m_view.getCenter() }
    };
    mouseViewPosition.floor();

    std::stringstream infoOutput {};
    infoOutput << "Tile Selected : " << this->m_tileSelector.get_tile_selected()
               << "\n";
    infoOutput << "mouse general position : " << mousePosition << "\n";
    infoOutput << "mouse view position : " << mouseViewPosition << "\n";
    infoOutput << "tilemap position : " << this->getPosition() << "\n";
    infoOutput << "tilemap size : " << this->get_size() << "\n";
    infoOutput << "tilemap - number of tile : "
               << this->get_size() / TILE_PIXEL_SIZE << "\n";
    infoOutput << "view center : " << this->m_view.getCenter() << "\n";
    infoOutput << "view size : " << this->m_view.getSize() << "\n";
    infoOutput << "view zoom : " << viewZoom << "\n";
    ImGui::Text( "%s", infoOutput.str().c_str() );

    if ( mouseViewPosition.is_inside( this->getPosition(), this->get_size() ) )
    {
        math::Vector2F const mouseRelativPosition {
            mouseViewPosition - math::Vector2F { this->getPosition() }
        };
        math::Vector2F const tileSelectedPositionInPixel {
            mouseRelativPosition - ( mouseRelativPosition % TILE_PIXEL_SIZE_U )
        };
        math::Vector2U const tileSelectedPositionInTile {
            tileSelectedPositionInPixel / TILE_PIXEL_SIZE_U
        };

        std::stringstream selectionOutput {};
        selectionOutput << "mouse relativ position : " << mouseRelativPosition
                        << "\n";
        selectionOutput << "middle : "
                        << ( mouseRelativPosition % TILE_PIXEL_SIZE_U ) << "\n";
        selectionOutput << "tile selected position in pixel : "
                        << tileSelectedPositionInPixel << "\n";
        selectionOutput << "tile selected position in tile : "
                        << tileSelectedPositionInTile << "\n";
        ImGui::Text( "%s", selectionOutput.str().c_str() );

        if ( this->m_isLeftButtonPressed
             && this->m_tileSelector.get_tile_selected() >= 0 )
        {
            this->change_tile( tileSelectedPositionInTile,
                               this->m_tileSelector.get_tile_selected() );
        }

        this->m_cursor.setOutlineColor( sf::Color::Black );
        this->m_cursor.setPosition(
            math::Vector2F { tileSelectedPositionInPixel } );
    }
    else
    {
        this->m_cursor.setOutlineColor( sf::Color::Transparent );
    }
}
void TileMap::update_table_informations()
{
    unsigned int tileSizeX { static_cast< unsigned int >(
        this->get_tile_size().x ) };
    unsigned int tileSizeY { static_cast< unsigned int >(
        this->get_tile_size().y ) };

    static std::stringstream tileValueStream {};

    if ( ImGui::BeginTable( "table_padding", static_cast< int >( tileSizeX ) ) )
    {
        for ( unsigned int line { 0u }; line < tileSizeY; ++line )
        {
            ImGui::TableNextRow();
            for ( unsigned int column { 0u }; column < tileSizeX; ++column )
            {
                ImGui::TableSetColumnIndex( static_cast< int >( column ) );
                std::stringstream buttonStream {};
                buttonStream
                    << math::Vector2U { column, line } << " = "
                    << this->m_tileTable[line][column][this->m_currentDepth]
                           .get_value();
                if ( ImGui::Button( buttonStream.str().c_str() ) )
                {
                    // Clear the stream
                    tileValueStream.str( "" );

                    for ( Tile const & element :
                          this->m_tileTable[line][column] )
                    {
                        tileValueStream << element << "\n";
                    }
                }
            }
        }
        ImGui::EndTable();
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
        char * tileNumberXEnd;
        char * tileNumberYEnd;
        this->set_tile_size( math::Vector2S {
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