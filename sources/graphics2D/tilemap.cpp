#include "tilemap.hpp"

#include "tilemap/tile_utility.hpp"
#include "tools/databases.hpp"
#include "tools/global_variable.hpp"
#include "tools/imgui.hpp"
#include "tools/json.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

TileMap::TileMap( Tileset const & tileset )
  : m_tileset( tileset ), m_currentDepth( 0u )
{
    // json const tilemapRequest { db::request(
    //     "SELECT table_tilemap FROM tilemap;"s ) };

    // // TYPO pourquoi on doit acceder à [0]["table_tilemap"] pour avoir la valeur
    // json const jsonTilemap { json::parse(
    //     std::string { tilemapRequest[0]["table_tilemap"] } ) };

    // this->set_tile_table( jsonaddon::decode_array( jsonTilemap ) );

    this->set_tile_table( {
        {{ 0 }, { 2 }},
        {{ 2 }, { 0 }}
    } );

    this->setPosition( 0.f, 0.f );
}

sf::Vector2f TileMap::get_size() const
{
    // TYPO il faut verifier que toute les ligne de toutes les colonnes fasse la même taille
    return sf::Vector2f { static_cast< float >( this->m_tileTable[0][0].size() )
                              * TILE_PIXEL_SIZE,
                          static_cast< float >( this->m_tileTable[0].size() )
                              * TILE_PIXEL_SIZE };
}

void TileMap::update() {}

math::Rectangle get_tile_rectangle_in_tilemap(
    math::Vector2D const & tilemapPosition,
    math::Vector2D const & tileCoordinate )
{
    math::Rectangle rectangle {};

    rectangle.position = tilemapPosition + ( tileCoordinate * TILE_PIXEL_SIZE );
    rectangle.size     = TILE_PIXEL_SIZE_VECTOR;

    return rectangle;
}

math::Rectangle get_tile_rectangle_in_texture( int const & tileValue,
                                               unsigned int numberOfTile )
{
    std::div_t divisionValue { std::div( tileValue,
                                         static_cast< int >( numberOfTile ) ) };

    return {
        math::Vector2D {divisionValue.quot, divisionValue.rem}
            * TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE_VECTOR
    };
}

void set_vertex_array( sf::VertexArray & vertexArray,
                       math::Vector2D const & tilemapPosition,
                       math::Vector2D const & tileCoordinate )
{
    math::Rectangle const rectangle {
        get_tile_rectangle_in_tilemap( tilemapPosition, tileCoordinate )
    };

    vertexArray[0].position = rectangle.position;
    vertexArray[1].position =
        rectangle.position + math::Vector2D { rectangle.size.x, 0.f };
    vertexArray[2].position =
        rectangle.position + math::Vector2D { 0.f, rectangle.size.y };
    vertexArray[3].position = rectangle.position + rectangle.size;

    std::cout << "rectangle.position : " << rectangle.position << std::endl;
    std::cout << "rectangle.size : " << rectangle.size << std::endl;
}

void set_texture_coordinate( sf::VertexArray & vertexArray,
                             int const & tileValue,
                             unsigned int numberOfXAxisTile )
{
    math::Rectangle textureTileRectangle {
        get_tile_rectangle_in_texture( tileValue, numberOfXAxisTile )
    };

    vertexArray[0].texCoords = textureTileRectangle.position;
    vertexArray[1].texCoords =
        textureTileRectangle.position
        + math::Vector2D { textureTileRectangle.size.x, 0.f };
    vertexArray[2].texCoords =
        textureTileRectangle.position
        + math::Vector2D { 0.f, textureTileRectangle.size.y };
    vertexArray[3].texCoords =
        textureTileRectangle.position + textureTileRectangle.size;

    std::cout << "textureTileRectangle.position : "
              << textureTileRectangle.position << std::endl;
    std::cout << "textureTileRectangle.size : " << textureTileRectangle.size
              << std::endl;
}

void TileMap::set_tile_table(
    std::vector< std::vector< std::vector< int > > > const & table )
{
    // Parse all the column of the tilemap
    for ( unsigned int line { 0u }; line < table.size(); ++line )
    {
        this->m_tileTable.push_back( {} );

        // Parse all the tile of the tilemap
        for ( unsigned int row { 0u }; row < table[line].size(); ++row )
        {
            this->m_tileTable[line].push_back( {} );

            // Parse each cell of each tile of the tilemap
            for ( unsigned int depth { 0u }; depth < table[line][row].size();
                  ++depth )
            {
                // We add the element to the tile table
                this->m_tileTable[line][row].push_back( S_TileData {
                    table[line][row][depth],
                    sf::VertexArray {sf::Quads, 4}
                } );

                S_TileData & tile { this->m_tileTable[line][row][depth] };

                math::Vector2D const currentTilePosition { row, line };

                std::cout << "currentTilePosition : " << currentTilePosition
                          << std::endl;

                set_vertex_array( tile.vertice,
                                  this->getPosition(),
                                  currentTilePosition );

                set_texture_coordinate(
                    tile.vertice,
                    tile.value,
                    this->m_tileset.get_texture().getSize().x
                        / TILE_PIXEL_SIZE_U );
            }
        }
    }
}

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    states.texture = &this->m_tileset.get_texture();

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

// TileMapEditor::TileMapEditor( sf::Texture const & texture )
//   : TileMap( texture ), m_currentDepth( 0u )
// {}

// void TileMapEditor::change_tile( sf::Vector2u const /* tilePositionInTile */,
//                                  unsigned int const /* newTile */ )
// {
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
// }

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

// void TileMapEditor::update() {}