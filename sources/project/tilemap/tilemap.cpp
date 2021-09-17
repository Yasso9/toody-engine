#include "tilemap.hpp"

TileMap::TileMap() : m_fileName( "./ressources/tilemap.txt" )
{
    this->load();
    this->reset();
}

/* Charge tout les informations de la tilemap contenu dans le fichier */
void TileMap::load()
{
    /* Contient toute la chaine du fichier */
    std::string const fileContent { read_file( this->m_fileName ) };

    /* Indice de caractère général dans la lecture de la chaine */
    unsigned int index { 0 };
    // Boucle générale de lecture de plusieurs tilemaps
    for ( unsigned int tableIndex { 0 }; tableIndex < this->m_table.size(); ++tableIndex )
    {
        std::string definition { read_definition( fileContent, index ) };

        // Verification de la validité de la définition
        // tableIndex commence a partir de 0
        if ( definition != "tilemap" + std::to_string( tableIndex + 1 ) )
        {
            throw std::invalid_argument( "Definition : \"" + definition +
            " is not handled. We must have \"tilemap" + std::to_string( tableIndex ) );
        }

        this->m_table[tableIndex] = read_tilemap( fileContent, index, this->m_tileNumber, tableIndex );
    }

    this->m_size = sf::Vector2f( this->m_tileNumber ) * g_squareSize;
}

void TileMap::reset()
{
    for ( unsigned int tilemapNumber { 0 }; tilemapNumber < this->m_vertices.size(); ++tilemapNumber )
    {
        this->m_vertices[tilemapNumber].setPrimitiveType( sf::Quads );
        this->m_vertices[tilemapNumber].resize( this->m_tileNumber.x * this->m_tileNumber.y * 4 );
    }

    for ( unsigned int x { 0u }; x < this->m_tileNumber.x; ++x )
    {
        for ( unsigned int y { 0u }; y < this->m_tileNumber.y; ++y )
        {
            /* Numero de la tile */
            unsigned int number { x + y * this->m_tileNumber.x };

            // Boucle des différentent tilemap
            for ( unsigned int tilemapNumber { 0u }; tilemapNumber < this->m_vertices.size(); ++tilemapNumber )
            {
                sf::Vertex * quad = &this->m_vertices[tilemapNumber][(x + y * this->m_tileNumber.x) * 4];

                set_quad_position( quad, this->getPosition(), sf::Vector2u( x, y ) );

                // Pour une deuxième ou autre tilemap, -1 signifie que la case doit être invisible 
                if ( tilemapNumber == 0u || this->m_table[tilemapNumber][number] < 0 )
                {
                    sf::Vector2u const tilePosition {
                        to_tile_position(
                            static_cast<unsigned int>(this->m_table[tilemapNumber][number]),
                            this->m_tileNumber.x
                        )
                    };

                    set_quad_texture_coordinate( quad, tilePosition );
                }
                else
                {
                    set_quad_transparency( quad );
                }
            }
        }
    }
}

void TileMap::set_texture( sf::Texture const & texture )
{
    this->m_texture = std::make_shared<sf::Texture>( texture );
}

sf::Vector2f TileMap::get_size() const
{
    return this->m_size;
}

void TileMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= getTransform();

    states.texture = this->m_texture.get();

    for ( unsigned int tilemapNumber { 0u }; tilemapNumber < this->m_vertices.size(); ++tilemapNumber )
    {
        target.draw( this->m_vertices[tilemapNumber], states );
    }
}



EditorMap::EditorMap()
{
    this->m_isMouseOn = true;
    this->m_actualDepth = 0u;
}

void EditorMap::set_depth( unsigned int const & actualDepth )
{
    assert( actualDepth < g_totalDepth && "EditorMap::actualDepth can only be between 0 and g_totalDepth" );

    this->m_actualDepth = actualDepth;
}

unsigned int EditorMap::get_depth() const
{
    return this->m_actualDepth;
}

void EditorMap::save() const
{
    std::ofstream file { this->m_fileName, std::ios::out };
    if ( !file )
    {
        throw FileError( "File not found", this->m_fileName );
    }

    for ( unsigned int depth { 0 }; depth < g_totalDepth; ++depth )
    {
        file << "tilemap " << std::to_string( depth + 1 ) << " :\n";

        file << "| ";
        for ( std::size_t i { 0 }; i < this->m_table[0].size(); ++i )
        {
            file << this->m_table[depth][i] << " | ";

            // Dès qu'on arrive a la fin de la ligne, sauf à 
            // la dernière ligne car il faudra lire la prochaine tilemap
            if ( i % this->m_tileNumber.x == this->m_tileNumber.x - 1 && i != this->m_table[0].size() - 1 )
            {
                file << "\n";
                // On commence une nouvelle ligne
                file << "| ";
            }
        }
        file << "\n";
    }

    file.close();
}

void EditorMap::change_tile( sf::Vector2u const tilePosition, unsigned int const newTile )
{
    unsigned int const tilePositionValue { tilePosition.x + tilePosition.y * this->m_tileNumber.x };

    // On actualise les valeurs du tableau
    this->m_table[this->m_actualDepth][tilePositionValue] = static_cast<int>(newTile);

    sf::Vertex * quad = &this->m_vertices[this->m_actualDepth][tilePositionValue * 4];

    // On enlève la transparence du quad
    set_quad_visible( quad );

    set_quad_texture_coordinate( quad, to_tile_position( newTile, this->m_tileNumber.x ) );
}

void EditorMap::resize()
{
    // sf::Vector2u pastSize { this->m_tileNumber };

    // std::cout << "Entree la taille en x : ";
    // secure_entry( this->m_tileNumber.x );
    // std::cout << "Entree la taille en y : ";
    // secure_entry( this->m_tileNumber.y );
    // std::cout << std::endl;

    // if ( pastSize.x <= this->m_tileNumber.x && pastSize.y <= this->m_tileNumber.y )
    // {
    //     for ( unsigned int j { 0u }; j < pastSize.y; ++j )
    //     {
    //         for ( unsigned int i { pastSize.x }; i < this->m_tileNumber.x; ++i )
    //         {
    //             this->m_table[0].insert( this->m_table[0].begin() + i + j * this->m_tileNumber.x, 0 );
    //         }
    //     }
    //     for ( unsigned int j { 0u }; j < (this->m_tileNumber.y - pastSize.y) * this->m_tileNumber.x; ++j )
    //     {
    //         this->m_table[0].push_back( 0 );
    //     }

    // }
    // else if ( pastSize.x <= this->m_tileNumber.x && pastSize.y >= this->m_tileNumber.y )
    // {
    //     for ( unsigned int j { 0u }; j < this->m_tileNumber.y; ++j )
    //     {
    //         for ( unsigned int i { pastSize.x }; i < this->m_tileNumber.x; ++i )
    //         {
    //             this->m_table[0].insert( this->m_table[0].begin() + i + j * this->m_tileNumber.x, 0 );
    //         }
    //     }

    //     this->m_table[0].erase( this->m_table[0].begin() + this->m_tileNumber.x * this->m_tileNumber.y, this->m_table[0].end() );
    // }
    // else if ( pastSize.x >= this->m_tileNumber.x && pastSize.y <= this->m_tileNumber.y )
    // {
    //     for ( unsigned int j { 0u }; j < pastSize.y; ++j )
    //     {
    //         this->m_table[0].erase( this->m_table[0].begin() + this->m_tileNumber.x + j * this->m_tileNumber.x, this->m_table[0].begin() + pastSize.x + j * this->m_tileNumber.x );
    //     }
    //     for ( unsigned int j { 0.f }; j < (this->m_tileNumber.y - pastSize.y) * this->m_tileNumber.x; ++j )
    //     {
    //         this->m_table[0].push_back( 0 );
    //     }
    // }
    // else if ( pastSize.x >= this->m_tileNumber.x && pastSize.y >= this->m_tileNumber.y )
    // {
    //     for ( unsigned int j { 0u }; j < pastSize.y; ++j )
    //     {
    //         this->m_table[0].erase( this->m_table[0].begin() + this->m_tileNumber.x + j * this->m_tileNumber.x, this->m_table[0].begin() + pastSize.x + j * this->m_tileNumber.x );
    //     }

    //     this->m_table[0].erase( this->m_table[0].begin() + this->m_tileNumber.x * this->m_tileNumber.y, this->m_table[0].end() );
    // }

    // if ( this->m_table[0].size() != this->m_tileNumber.x * this->m_tileNumber.y )
    // {
    //     throw std::runtime_error( "table[0] this->m_tileNumber and tableSize.x * tableSize.y haven't the same value" );
    // }

    // // this->position = sf::Vector2f((WINDOW_WIDTH - (this->m_tileNumber.x * SQUARE)) / 2, (WINDOW_WIDTH - (this->m_tileNumber.y * SQUARE)) / 2 );
    // this->m_size = sf::Vector2f( this->m_tileNumber ) * 32.f;

    this->reset();
}


void EditorMap::update( sf::Vector2f const cursorPosition, unsigned int const newTile, bool const inputIsPress )
{
    // Si cursorPosition est dans la tilemap
    if ( is_in_part( cursorPosition, this->getPosition(), this->m_size ) )
    {
        this->m_isMouseOn = true;

        // Coordonnée de la tilemap où est placé le curseur
        sf::Vector2u const tilePosition { to_tile_position( cursorPosition - this->getPosition() ) };

        // L'utilisateur clique sur le bouton pour réaliser l'action
        if ( inputIsPress )
        {
            // On actualise l'ecran
            this->change_tile( tilePosition, newTile );
        }

        this->m_tileCursor.update( cursorPosition, this->getPosition() );

    }
    else
    {
        this->m_isMouseOn = false;

    }
}

void EditorMap::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= getTransform();

    states.texture = this->m_texture.get();

    for ( unsigned int tilemapNumber { 0 }; tilemapNumber < this->m_vertices.size(); ++tilemapNumber )
    {
        target.draw( this->m_vertices[tilemapNumber], states );
    }

    // On et aussi les tileCursor de la tilemap si il le faut
    if ( this->m_isMouseOn )
    {
        target.draw( this->m_tileCursor );
    }
}


// Test unitaire
// for(auto element : table)
// {
//         std::cout << "table" << std::endl;
//     unsigned int compteur { 0 };
//     for(auto value : tilemapTable)
//     {
//         std::cout << value << " ";
//         compteur++;
//         if(compteur == tilemapSize.x)
//         {
//             std::cout << "\n";
//             compteur = 0;
//         }
//     }
// }
// std::cout << tilemapSize << std::endl;
// std::cout << "\n" << std::endl;