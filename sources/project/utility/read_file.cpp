#include "read_file.hpp"

void erase_space( std::string & text )
{
    for ( unsigned int index { 0 }; index < text.size(); index++ )
    {
        if ( std::isspace( text[index] ) )
        {
            text.erase( index, 1 );
        }
    }
}

std::string read_file( std::string const & fileName )
{
    std::ifstream file { fileName, std::ios::in };
    if ( !file )
    {
        throw FileError( "File not found", fileName );
    }

    /* Contient tout le contenu du fichier */
    std::string fileContent { "" };
    // Contient juste les lignes qu'on va prendre une par une
    std::string tableLine { "" };
    while ( file.tellg() != -1 )
    {
        std::getline( file, tableLine );
        fileContent += tableLine;
    }

    file.close();

    // On ne s'embete pas avec les espaces inutile, 
    // il y'a de toute façon d'autres caractères de séparations
    erase_space( fileContent );

    return fileContent;
}

std::string read_definition( std::string const & fileContent, unsigned int & index )
{
    /* Doit contenir la valeur de la definition */
    std::string definition { "" };
    while ( fileContent[index] != ':' )
    {
        if ( index >= fileContent.size() )
        {
            throw std::invalid_argument( R"(Il n'y a pas de ':' qui permettent 
            de distinguer une definition. La definition actuel est egal a : ")"
                + definition + "\"" );
        }

        definition.push_back( fileContent[index] );
        index++;
    }

    // Le caractère actuel est ":", on passe au suivant
    index++;

    return definition;
}

int read_number( std::string const & fileContent, unsigned int & index )
{
    std::string temp { "" };

    // Boucle de lecture de nombres
    while ( fileContent[index] != '|' )
    {
        if ( index >= fileContent.size() )
        {
            throw std::invalid_argument( "End of file while we must read a number" );
        }
        else if ( !std::isdigit( fileContent[index] ) )
        {
            throw std::invalid_argument( "Non-number value in tilemap : " +
                std::to_string( fileContent[index] ) + " - in position : " + std::to_string( index ) );
        }

        temp.push_back( fileContent[index] );
        index++;
    }

    return std::stoi( temp );
}

void vericafication_string_inconsistency( unsigned int const & loopIterator,
    unsigned int & lastValue, unsigned int & newValue, std::string const & coordinate )
{
    // Cas ou on est soit dans la première ligne soit la verification est bonne
    if ( loopIterator == 0 || lastValue == newValue )
    {
        lastValue = newValue;
        newValue = 0;
    }
    else
    {
        std::cout << loopIterator << coordinate << std::endl;
        throw std::invalid_argument( "Different size in " + coordinate + " coordinnate in the tilemap file : " +
            std::to_string( lastValue ) + " and " + std::to_string( newValue ) );
    }
}

void read_tilemap_line( std::string const & fileContent, unsigned int & index,
    std::vector<int> & tilemapTable, sf::Vector2u & verificationSize,
    FileStep & FileStep, bool shouldReadNumber )
{
    // Boucle de lecture de nombres horizontale (en X)
    while ( FileStep == FileStep::SameLine )
    {
        // On a fini de lire le fichier, on quitte la boucle 
        // et on procéde aux verifications
        if ( index >= fileContent.size() )
        {
            FileStep = FileStep::End;
        }
        // On doit passer au caractère suivant
        else if ( fileContent[index] == '|' )
        {
            // Cas ou il y'a une alternance normal entre une barre et un nombre
            if ( !shouldReadNumber )
            {
                index++;
                shouldReadNumber = true;
            }
            // Cas ou il y'a deux barres d'affilées. 
            else
            {
                FileStep = FileStep::NextLine;
                // A chaque nouvelle ligne on doit commencé par une barre
                shouldReadNumber = false;
            }
        }
        // Si on lit une lettre c'est qu'il y a une nouvelle définition
        else if ( std::isalpha( fileContent[index] ) )
        {
            FileStep = FileStep::NextDefinition;
        }
        // On doit lire un nombre
        else if ( std::isdigit( fileContent[index] ) )
        {
            if ( !shouldReadNumber )
            {
                throw std::invalid_argument( "We have a number without having a '|' in position : "
                    + std::to_string( index ) );
            }
            // Il n'y a pas de barre, à la prochaine itération 
            // il doit normalement y en avoir une. 
            shouldReadNumber = false;

            tilemapTable.push_back( read_number( fileContent, index ) );

            // Il y'a un nombre en plus dans l'axe X
            verificationSize.x++;
        }
        else
        {
            throw std::invalid_argument( "Character not handled by the program : \"" +
                std::to_string( fileContent[index] ) + "\"" );
        }
    }
}

std::vector<int> read_tilemap( std::string const & fileContent, unsigned int & index,
    sf::Vector2u & tilemapSize, unsigned int & superiorLoopIndex )
{
    std::vector<int> tilemapTable {};

    /* Retient la taille de chaque nouvelle ligne et colonne pour
    s'assurer que la tilemap est un rectangle constant */
    sf::Vector2u verificationSize { sf::Vector2u( 0, 0 ) };

    /* Retient le fait qu'on doit avoir soit un nombre (true)
    soit une barre (false) à l'endroit ou est le curseur qui lit la chaine
    A chaque nouvelle tilemap il faut commencer par une barre */
    bool shouldReadNumber { false };

    /* Dit la prochaine étape à lire dans le fichier
    (voir enumération pour connaitre les étapes) */
    FileStep FileStep { FileStep::SameLine };

    // Boucle de lecture de ligne vertical (en Y)
    while ( FileStep != FileStep::NextDefinition && FileStep != FileStep::End && index < fileContent.size() )
    {
        // On doit lire une ligne
        FileStep = FileStep::SameLine;

        read_tilemap_line(
            fileContent, index, tilemapTable,
            verificationSize, FileStep, shouldReadNumber
        );

        vericafication_string_inconsistency( verificationSize.y, tilemapSize.x, verificationSize.x, "x" );

        // On a fini cette ligne là on passe à la ligne suivante
        verificationSize.y++;
    }

    vericafication_string_inconsistency( superiorLoopIndex, tilemapSize.y, verificationSize.y, "y" );


    // std::cout << "table" << std::endl;
    // unsigned int compteur { 0 };
    // for(auto value : tilemapTable)
    // {
    //     std::cout << value << " ";
    //     compteur++;
    //     if(compteur == tilemapSize.x)
    //     {
    //         std::cout << "\n";
    //         compteur = 0;
    //     }
    // }
    // std::cout << "\n" << std::endl;
    // std::cout << tilemapSize << std::endl;

    return tilemapTable;
}