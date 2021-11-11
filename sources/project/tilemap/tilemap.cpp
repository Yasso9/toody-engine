#include "tilemap.hpp"

TileMap::TileMap( sf::Texture const & texture ) : m_texture( texture )
{
    json const completeResult = database::request( R"(
    SELECT table_tilemap FROM tilemap;
    )"s );

    json const singleValue { json::parse(
        std::string { completeResult[0]["table_tilemap"] } ) };

    this->m_table = jsonaddon::decode_array( singleValue );
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

                set_quad_position( quad,
                                   this->getPosition(),
                                   sf::Vector2u( line, row ) );

                sf::Vector2u const tilePosition { to_tile_position(
                    this->m_table[row][line][tileDepth],
                    this->m_table[row].size() ) };

                set_quad_texture_coordinate( quad, tilePosition );
            }
        }
    }
}

sf::Vector2f TileMap::get_size() const
{
    // TYPO il faut verifier que toute les ligne de toutes les colonnes fasse la même taille
    return sf::Vector2f {
        static_cast<float>( m_vertices[0][0].size() ) * g_squareSize,
        static_cast<float>( m_vertices[0].size() ) * g_squareSize
    };
}

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= getTransform();

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

EditorMap::EditorMap( sf::Texture const & texture ) : TileMap( texture )
{
    this->m_isMouseOn = true;
    this->m_actualDepth = 0u;
}

void EditorMap::set_actual_depth( unsigned int const & actualDepth )
{
    // TYPO à voir si on garde
    // assert(
    //     actualDepth < g_totalDepth
    //     && "EditorMap::actualDepth can only be between 0 and g_totalDepth" );

    this->m_actualDepth = actualDepth;
}

unsigned int EditorMap::get_actual_depth() const
{
    return this->m_actualDepth;
}

void EditorMap::save() const
{
    json tilemapSave {};
    tilemapSave["array"] = this->m_table;

    database::request( R"(
    INSERT INTO tilemap( table_tilemap )
    VALUES( ")"s + tilemapSave["array"].dump()
                       +
                       R"(" );
    )"s );
}

void EditorMap::change_tile( sf::Vector2u const /* tilePosition */,
                             unsigned int const /* newTile */ )
{
    // unsigned int const tilePositionValue {
    //     tilePosition.x + tilePosition.y * this->m_table[0][0].size()
    // };

    // // On actualise les valeurs du tableau
    // this->m_table[this->m_actualDepth][tilePositionValue] =
    //     static_cast<int>( newTile );

    // sf::Vertex * quad =
    //     &this->m_vertices[this->m_actualDepth][tilePositionValue * 4];

    // // On enlève la transparence du quad
    // set_quad_visible( quad );

    // set_quad_texture_coordinate(
    //     quad,
    //     to_tile_position( newTile, this->m_table[0][0].size() ) );
}

void EditorMap::update( sf::Vector2f const /* cursorPosition */,
                        unsigned int const /* newTile */,
                        bool const /* inputIsPress */ )
{
    // // Si cursorPosition est dans la tilemap
    // if ( is_in_part( cursorPosition, this->getPosition(), this->m_size ) )
    // {
    //     this->m_isMouseOn = true;

    //     // Coordonnée de la tilemap où est placé le curseur
    //     sf::Vector2u const tilePosition { to_tile_position(
    //         cursorPosition - this->getPosition() ) };

    //     // L'utilisateur clique sur le bouton pour réaliser l'action
    //     if ( inputIsPress )
    //     {
    //         // On actualise l'ecran
    //         this->change_tile( tilePosition, newTile );
    //     }

    //     this->m_tileCursor.update( cursorPosition, this->getPosition() );
    // }
    // else
    // {
    //     this->m_isMouseOn = false;
    // }
}

void EditorMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    TileMap::draw( target, states );

    // On et aussi les tileCursor de la tilemap si il le faut
    if ( this->m_isMouseOn )
    {
        target.draw( this->m_tileCursor );
    }
}