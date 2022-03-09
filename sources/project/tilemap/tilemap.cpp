#include "tilemap.hpp"

#include "tilemap/tile_utility.hpp"
#include "tools/databases.hpp"
#include "tools/global_variable.hpp"
#include "tools/json.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

TileMap::TileMap( sf::Texture const & texture ) : m_texture( texture )
{
    json const completeResult = db::request( R"(
    SELECT table_tilemap FROM tilemap;
    )"s );

    json const singleValue { json::parse(
        std::string { completeResult[0]["table_tilemap"] } ) };

    this->m_table = jsonaddon::decode_array( singleValue );

    this->synchronize_vertices();
}

void TileMap::synchronize_vertices()
{
    // Parse all the column of the tilemap
    for ( unsigned int row { 0u }; row < this->m_table.size(); ++row )
    {
        this->m_vertices.push_back( {} );

        // Parse all the tile of the tilemap
        for ( unsigned int line { 0u }; line < this->m_table[row].size();
              ++line )
        {
            this->m_vertices[row].push_back( {} );

            // Parse each cell of each tile of the tilemap
            for ( unsigned int tileDepth { 0u };
                  tileDepth < this->m_table[row][line].size();
                  ++tileDepth )
            {
                this->m_vertices[row][line].push_back(
                    sf::VertexArray { sf::Quads, 4 } );

                sf::VertexArray & quad {
                    this->m_vertices[row][line][tileDepth]
                };

                quad::set_position( quad,
                                    this->getPosition(),
                                    sf::Vector2u( line, row ) );

                sf::Vector2u const tilePosition { to_tile_position(
                    this->m_table[row][line][tileDepth],
                    static_cast<unsigned int>( this->m_table[row].size() ) ) };

                quad::set_texture_coordinate( quad, tilePosition );
            }
        }
    }

    this->setPosition( 0.f, 0.f );
}

sf::Vector2f TileMap::get_size() const
{
    // TYPO il faut verifier que toute les ligne de toutes les colonnes fasse la même taille
    return sf::Vector2f {
        static_cast<float>( m_vertices[0][0].size() ) * ::g_tileSize_f,
        static_cast<float>( m_vertices[0].size() ) * ::g_tileSize_f
    };
}

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    // TYPO tester si c'est utile
    states.texture = &this->m_texture;

    for ( auto const & row : this->m_vertices )
    {
        for ( auto const & tile : row )
        {
            for ( auto const & cell : tile )
            {
                target.draw( cell, states );
            }
        }
    }
}

EditorMap::EditorMap( sf::Texture const & texture )
  : TileMap( texture ), m_cursor(), m_depth( 0u )
{}

void EditorMap::set_depth( unsigned int const & actualDepth )
{
    // TYPO à voir si on garde
    // assert(
    //     actualDepth < g_totalDepth
    //     && "EditorMap::actualDepth can only be between 0 and g_totalDepth" );

    this->m_depth = actualDepth;
}

unsigned int EditorMap::get_depth() const
{
    return this->m_depth;
}

void EditorMap::save() const
{
    json tilemapSave {};
    tilemapSave["array"] = this->m_table;

    db::request( R"(
    INSERT INTO tilemap( table_tilemap )
    VALUES( ")"s + tilemapSave["array"].dump()
                 +
                 R"(" );
    )"s );
}

void EditorMap::change_tile( sf::Vector2u const tilePosition,
                             unsigned int const newTile )
{
    // On actualise les valeurs du tableau
    this->m_table[this->m_depth][tilePosition.y][tilePosition.x] = newTile;

    sf::VertexArray & quad =
        this->m_vertices[this->m_depth][tilePosition.y][tilePosition.x];

    // On enlève la transparence du quad
    quad::set_visible( quad );

    // TYPO c'est foireux : faut mettre le tilePosition de newtile (la tileset et pas la tilemap)
    quad::set_texture_coordinate( quad, tilePosition );
}

void EditorMap::update( sf::Vector2f const cursorPosition,
                        unsigned int const newTile, bool const inputIsPress )
{
    // Si cursorPosition est dans la tilemap
    if ( tools::is_in_part( cursorPosition,
                            this->getPosition(),
                            this->get_size() ) )
    {
        // Coordonnée de la tilemap où est placé le curseur
        sf::Vector2u const tilePosition { to_tile_position(
            cursorPosition - this->getPosition() ) };

        // L'utilisateur clique sur le bouton pour réaliser l'action
        if ( inputIsPress )
        {
            // On actualise l'ecran
            this->change_tile( tilePosition, newTile );
        }

        this->m_cursor.update( cursorPosition, this->getPosition() );
    }
}

void EditorMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    TileMap::draw( target, states );
    target.draw( this->m_cursor );
}