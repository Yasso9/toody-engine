#include <stdlib.h>  // for EXIT_SUCCESS

#include "main/game.hpp"  // for Game
#include "tools/databases.hpp"

int main ()
{
    // Game game {};

    // game.run();

    db::test_table();

    return EXIT_SUCCESS;
}