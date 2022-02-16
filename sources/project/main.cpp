#include "main/game.hpp"

int main()
{
    // TYPO mettre tous les databases dans la bonne place
    // database::request( R"(
    // DROP TABLE IF EXISTS tilemap;
    // CREATE TABLE tilemap(
    //     table_tilemap TEXT NOT NULL
    // );
    // )"s );

    // std::vector<std::vector<std::vector<unsigned int>>> tripleArray {
    //     {
    //         { 5, 3, 4, 4, 2, 5 },
    //         { 5, 3, 4, 4, 2, 5 },
    //         { 5, 3, 4, 4, 2, 5 },
    //     },
    //     {
    //         { 5, 3, 4, 4, 2, 5 },
    //         { 5, 3, 4, 4, 2, 5 },
    //         { 5, 3, 4, 4, 2, 5 },
    //     }
    // };

    // json jsonTest {};
    // jsonTest["array"] = tripleArray;

    // database::request( R"(
    // INSERT INTO tilemap( table_tilemap )
    // VALUES( ")"s + jsonTest["array"].dump()
    //                    +
    //                    R"(" );
    // INSERT INTO tilemap( table_tilemap )
    // VALUES( ")"s + jsonTest["array"].dump()
    //                    +
    //                    R"(" );
    // INSERT INTO tilemap( table_tilemap )
    // VALUES( ")"s + jsonTest["array"].dump()
    //                    +
    //                    R"(" );
    // )"s );

    Game game {};

    game.run();

    // OpenGL::graphics();

    return EXIT_SUCCESS;
}