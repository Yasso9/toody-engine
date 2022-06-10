#include "tilemap.hpp"

#include "main/window.hpp"
#include "tilemap/tile_utility.hpp"
#include "tools/databases.hpp"
#include "tools/global_variable.hpp"
#include "tools/imgui.hpp"
#include "tools/json.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

static math::Rectangle get_tile_rectangle_in_tilemap(
    math::Vector2F const & tilemapPosition,
    math::Vector2F const & tileCoordinate )
{
    math::Rectangle rectangle {};

    rectangle.position = tilemapPosition + ( tileCoordinate * TILE_PIXEL_SIZE );
    rectangle.size     = TILE_PIXEL_SIZE_VECTOR;

    return rectangle;
}

static math::Rectangle get_tile_rectangle_in_texture(
    int const & tileValue, unsigned int numberOfTile )
{
    std::div_t divisionValue { std::div( tileValue,
                                         static_cast< int >( numberOfTile ) ) };

    return {
        math::Vector2F {divisionValue.rem, divisionValue.quot}
            * TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE_VECTOR
    };
}

TileQuad::TileQuad() : m_vextexArray( sf::Quads, 4 ) {}

sf::VertexArray const & TileQuad::get_vertex_array() const
{
    return this->m_vextexArray;
}

void TileQuad::set_position( math::Vector2F const & tilemapPosition,
                             math::Vector2F const & tileCoordinate )
{
    math::Rectangle const rectangle {
        get_tile_rectangle_in_tilemap( tilemapPosition, tileCoordinate )
    };

    this->m_vextexArray[0].position = rectangle.position;
    this->m_vextexArray[1].position =
        rectangle.position + math::Vector2F { rectangle.size.x, 0.f };
    this->m_vextexArray[2].position = rectangle.position + rectangle.size;
    this->m_vextexArray[3].position =
        rectangle.position + math::Vector2F { 0.f, rectangle.size.y };
}

void TileQuad::set_texture_coordinate( int const & tileValue,
                                       unsigned int numberOfXAxisTile )
{
    math::Rectangle textureTileRectangle {
        get_tile_rectangle_in_texture( tileValue, numberOfXAxisTile )
    };

    this->m_vextexArray[0].texCoords = textureTileRectangle.position;
    this->m_vextexArray[1].texCoords =
        textureTileRectangle.position
        + math::Vector2F { textureTileRectangle.size.x, 0.f };
    this->m_vextexArray[2].texCoords =
        textureTileRectangle.position + textureTileRectangle.size;
    this->m_vextexArray[3].texCoords =
        textureTileRectangle.position
        + math::Vector2F { 0.f, textureTileRectangle.size.y };
}

sf::Vertex TileQuad::operator[]( std::size_t index ) const
{
    ASSERTION( index <= 4, "Index must be between 0 and 4" );

    return this->m_vextexArray[index];
}

TileMap::TileMap( sf::View & view )
  : m_tileSelector(),
    m_cursor(),
    m_view( view ),
    m_tileTable(),
    m_currentDepth( 0u )
{
    // json const tilemapRequest { db::request(
    //     "SELECT table_tilemap FROM tilemap;"s ) };

    // /// @todo pourquoi on doit acceder à [0]["table_tilemap"] pour avoir la valeur
    // json const jsonTilemap { json::parse(
    //     std::string { tilemapRequest[0]["table_tilemap"] } ) };

    // this->set_tile_table( jsonaddon::decode_array( jsonTilemap ) );

    this->set_tile_table( {
        {{ 0 }, { 2 }},
        {{ 2 }, { 0 }}
    } );

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

math::Vector2F TileMap::get_tile_size() const
{
    /// @todo normalement le static cast ne sert à rien
    return math::Vector2F {
        static_cast< unsigned int >( this->m_tileTable[0].size() ),
        static_cast< unsigned int >( this->m_tileTable.size() )
    };
}

void TileMap::update()
{
    this->m_tileSelector.update();

    if ( ImGui::Begin( "Tilemap Information" ) )
    {
        math::Vector2F const mousePosition { ImGui::GetMousePos() };
        math::Vector2F const viewZoom { Window::get_instance().get_size_f()
                                        / this->m_view.getSize() };
        math::Vector2F mouseViewPosition { ( mousePosition / viewZoom )
                                           - ( this->m_view.getSize() / 2.f )
                                           + this->m_view.getCenter() };
        mouseViewPosition.floor();

        std::stringstream infoOutput {};
        infoOutput << "Tile Selected : "
                   << this->m_tileSelector.get_tile_selected() << "\n";
        infoOutput << "mouse general position : " << mousePosition << "\n";
        infoOutput << "mouse view position : " << mouseViewPosition << "\n";
        infoOutput << "tilemap position : " << this->getPosition() << "\n";
        infoOutput << "tilemap size : " << this->get_size() << "\n";
        infoOutput << "tilemap - number of tile : "
                   << this->get_size() / TILE_PIXEL_SIZE_U << "\n";
        infoOutput << "view center : " << this->m_view.getCenter() << "\n";
        infoOutput << "view size : " << this->m_view.getSize() << "\n";
        infoOutput << "view zoom : " << viewZoom << "\n";
        ImGui::Text( "%s", infoOutput.str().c_str() );

        if ( mouseViewPosition.is_inside( this->getPosition(),
                                          this->get_size() ) )
        {
            math::Vector2F const mouseRelativPosition { mouseViewPosition
                                                        - this->getPosition() };
            math::Vector2F const tileSelectedPosition {
                mouseRelativPosition
                - ( mouseRelativPosition % TILE_PIXEL_SIZE_U )
            };

            std::stringstream selectionOutput {};
            selectionOutput
                << "mouse relativ position : " << mouseRelativPosition << "\n";
            selectionOutput
                << "middle : " << ( mouseRelativPosition % TILE_PIXEL_SIZE_U )
                << "\n";
            selectionOutput
                << "tile selected position : " << tileSelectedPosition << "\n";
            ImGui::Text( "%s", selectionOutput.str().c_str() );

            if ( sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) )
            {
                this->change_tile( this->m_tileSelector.get_tile_selected(),
                                   tileSelectedPosition );
            }

            this->m_cursor.setOutlineColor( sf::Color::Black );
            this->m_cursor.setPosition( tileSelectedPosition );
        }
        else
        {
            this->m_cursor.setOutlineColor( sf::Color::Transparent );
        }

        unsigned int tileSizeX { static_cast< unsigned int >(
            this->get_tile_size().x ) };
        unsigned int tileSizeY { static_cast< unsigned int >(
            this->get_tile_size().y ) };

        static std::vector< S_TileData > tileValueArray {};

        if ( ImGui::BeginTable( "table_padding",
                                static_cast< int >( tileSizeX ) ) )
        {
            for ( unsigned int line { 0u }; line < tileSizeY; ++line )
            {
                ImGui::TableNextRow();
                for ( unsigned int column { 0u }; column < tileSizeX; ++column )
                {
                    ImGui::TableSetColumnIndex( static_cast< int >( column ) );
                    std::stringstream buttonStream {};
                    buttonStream << math::Vector2F { column, line } << " = "
                                 << this->m_tileTable[line][column][0].value;
                    if ( ImGui::Button( buttonStream.str().c_str() ) )
                    {
                        tileValueArray = this->m_tileTable[line][column];
                    }
                }
            }
            ImGui::EndTable();
        }

        if ( ! tileValueArray.empty() )
        {
            std::stringstream tileValueStream {};
            for ( auto const & element : tileValueArray )
            {
                tileValueStream << "Tile " << element.value << "\n";
                tileValueStream << "Position [ " << element.quad[0].position
                                << ", " << element.quad[1].position << ", "
                                << element.quad[2].position << ", "
                                << element.quad[3].position << " ]"
                                << "\n";
                tileValueStream << "TextCoord [ " << element.quad[0].texCoords
                                << ", " << element.quad[1].texCoords << ", "
                                << element.quad[2].texCoords << ", "
                                << element.quad[3].texCoords << " ]"
                                << "\n";
                tileValueStream << "\n";
            }
            ImGui::Text( "%s", tileValueStream.str().c_str() );
        }
    }
    ImGui::End();
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
                S_TileData tile { table[line][column][depth], TileQuad {} };

                // Not in pixel position
                math::Vector2F const currentTilePosition { column, line };

                tile.quad.set_position( this->getPosition(),
                                        currentTilePosition );

                tile.quad.set_texture_coordinate(
                    tile.value,
                    /// @todo simplify static_cast
                    static_cast< unsigned int >(
                        this->m_tileSelector.get_tileset()
                            .get_size_in_tile()
                            .x ) );

                // We add the element to the tile table
                this->m_tileTable[line][column].push_back( tile );
            }
        }
    }
}

void TileMap::change_tile( int const & newTileValue,
                           math::Vector2F const & tilePositionInTile )
{
    if ( newTileValue < 0 )
    {
        std::cout << "Tile negatif not handled" << std::endl;
        return;
    }

    /// @todo simplify static_cast
    S_TileData & tileData {
        this->m_tileTable[static_cast< unsigned int >( tilePositionInTile.y )]
                         [static_cast< unsigned int >( tilePositionInTile.x )]
                         [this->m_currentDepth]
    };

    /// @todo simplify static_cast
    tileData.quad.set_texture_coordinate(
        newTileValue,
        static_cast< unsigned int >(
            this->m_tileSelector.get_tileset().get_size_in_tile().x ) );

    tileData.value = newTileValue;
}

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    states.texture = &this->m_tileSelector.get_tileset().get_texture();

    for ( auto const & column : this->m_tileTable )
    {
        for ( auto const & tile : column )
        {
            for ( S_TileData const & cell : tile )
            {
                target.draw( cell.quad.get_vertex_array(), states );
            }
        }
    }

    target.draw( this->m_cursor, states );
}

// void TileMapEditor::save() const
// {
// json tilemapSave {};
// tilemapSave["array"] = this->m_table;

// db::request( R"(
// INSERT INTO tilemap( table_tilemap )
// VALUES( ")"s + tilemapSave["array"].dump()
//              +
//              R"(" );
// )"s );
// }