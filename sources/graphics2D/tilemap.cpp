#include "tilemap.hpp"

#include "tilemap/tile_utility.hpp"
#include "tools/databases.hpp"
#include "tools/global_variable.hpp"
#include "tools/imgui.hpp"
#include "tools/json.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

TileMap::TileMap( sf::Texture const & texture ) : m_texture( texture )
{
    json const tilemapRequest { db::request(
        "SELECT table_tilemap FROM tilemap;"s ) };

    // TYPO pourquoi on doit acceder à [0]["table_tilemap"] pour avoir la valeur
    json const jsonTilemap { json::parse(
        std::string { tilemapRequest[0]["table_tilemap"] } ) };

    this->set_tile_table( jsonaddon::decode_array( jsonTilemap ) );
}

sf::Vector2f TileMap::get_size() const
{
    // TYPO il faut verifier que toute les ligne de toutes les colonnes fasse la même taille
    return sf::Vector2f { static_cast< float >( this->m_tileTable[0][0].size() )
                              * TILE_PIXEL_SIZE,
                          static_cast< float >( this->m_tileTable[0].size() )
                              * TILE_PIXEL_SIZE };
}

void TileMap::set_tile_table(
    std::vector< std::vector< std::vector< int > > > const & table )
{
    // Parse all the column of the tilemap
    for ( unsigned int row { 0u }; row < table.size(); ++row )
    {
        this->m_tileTable.push_back( {} );

        // Parse all the tile of the tilemap
        for ( unsigned int line { 0u }; line < table[row].size(); ++line )
        {
            this->m_tileTable[row].push_back( {} );

            // Parse each cell of each tile of the tilemap
            for ( unsigned int depth { 0u }; depth < table[row][line].size();
                  ++depth )
            {
                // We add the element to the tile table
                this->m_tileTable[row][line].push_back( S_TileData {
                    table[row][line][depth],
                    sf::VertexArray {sf::Quads, 4}
                } );

                sf::Vector2f const tilePositionInTile {
                    static_cast< float >( line ),
                    static_cast< float >( row )
                };
                // sf::Vector2f const tilePositionInPixel { tilePositionInTile
                //                                          * TILE_PIXEL_SIZE };

                this->m_tileTable[row][line][depth].vertice[0].position =
                    this->getPosition()
                    + ( tilePositionInTile * TILE_PIXEL_SIZE );
                this->m_tileTable[row][line][depth].vertice[1].position =
                    sf::Vector2f(
                        this->getPosition().x
                            + ( tilePositionInTile.x + 1.f ) * TILE_PIXEL_SIZE,
                        this->getPosition().y
                            + tilePositionInTile.y * TILE_PIXEL_SIZE );
                this->m_tileTable[row][line][depth].vertice[2].position =
                    sf::Vector2f(
                        this->getPosition().x
                            + ( tilePositionInTile.x + 1.f ) * TILE_PIXEL_SIZE,
                        this->getPosition().y
                            + ( tilePositionInTile.y + 1.f )
                                  * TILE_PIXEL_SIZE );
                this->m_tileTable[row][line][depth].vertice[3].position =
                    sf::Vector2f( this->getPosition().x
                                      + tilePositionInTile.x * TILE_PIXEL_SIZE,
                                  this->getPosition().y
                                      + ( tilePositionInTile.y + 1.f )
                                            * TILE_PIXEL_SIZE );

                int tileSelected { this->m_tileTable[row][line][depth].value };

                std::div_t divisionValue { std::div(
                    tileSelected,
                    static_cast< int >( this->m_texture.getSize().x ) ) };

                sf::Vector2f texturePositionInTile {
                    static_cast< float >( divisionValue.quot ),
                    static_cast< float >( divisionValue.rem )
                };

                this->m_tileTable[row][line][depth].vertice[0].texCoords =
                    tilePositionInTile * TILE_PIXEL_SIZE;
                this->m_tileTable[row][line][depth].vertice[1].texCoords =
                    sf::Vector2f(
                        ( tilePositionInTile.x + 1.f ) * TILE_PIXEL_SIZE,
                        tilePositionInTile.y * TILE_PIXEL_SIZE );
                this->m_tileTable[row][line][depth].vertice[2].texCoords =
                    sf::Vector2f(
                        ( tilePositionInTile.x + 1.f ) * TILE_PIXEL_SIZE,
                        ( tilePositionInTile.y + 1.f ) * TILE_PIXEL_SIZE );
                this->m_tileTable[row][line][depth].vertice[3].texCoords =
                    sf::Vector2f(
                        tilePositionInTile.x * TILE_PIXEL_SIZE,
                        ( tilePositionInTile.y + 1.f ) * TILE_PIXEL_SIZE );
            }
        }
    }

    this->setPosition( 0.f, 0.f );
}

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    states.texture = &this->m_texture;

    for ( auto const & row : this->m_tileTable )
    {
        for ( auto const & tile : row )
        {
            for ( S_TileData const & cell : tile )
            {
                target.draw( cell.vertice, states );
            }
        }
    }
}

TileMapEditor::TileMapEditor( sf::Texture const & texture )
  : TileMap( texture ), m_currentDepth( 0u )
{}

void TileMapEditor::change_tile( sf::Vector2u const /* tilePositionInTile */,
                                 unsigned int const /* newTile */ )
{
    // // On actualise les valeurs du tableau
    // this->m_table[this->m_currentDepth][tilePositionInTile.y][tilePositionInTile.x] =
    //     newTile;

    // sf::VertexArray & quad {
    //     this->m_vertices[this->m_currentDepth][tilePositionInTile.y][tilePositionInTile.x]
    // };

    // // On enlève la transparence du quad
    // quad::set_visible( quad );

    // // TYPO c'est foireux : faut mettre le tilePositionInTile de newtile (la tileset et pas la tilemap)
    // quad::set_texture_coordinate( quad, tilePositionInTile );
}

void TileMapEditor::save() const
{
    // json tilemapSave {};
    // tilemapSave["array"] = this->m_table;

    // db::request( R"(
    // INSERT INTO tilemap( table_tilemap )
    // VALUES( ")"s + tilemapSave["array"].dump()
    //              +
    //              R"(" );
    // )"s );
}

void TileMapEditor::update()
{
    if ( ImGui::Begin( "Tilemap Editor" ) ) {}
    ImGui::End();
}