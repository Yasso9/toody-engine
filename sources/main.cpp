#include <stdlib.h>  // for EXIT_SUCCESS

#include "main/game.hpp"  // for Game
#include "tools/databases.hpp"

int main ()
{
    /// @todo remove all std::boolalpha ref in the project
    // Pretty print for boolean value
    std::cout << std::boolalpha;

    Game game {};

    game.run();

    // db::test_table();
    // test_serializer();

    return EXIT_SUCCESS;
}