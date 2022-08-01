#include <stdlib.h> // for EXIT_SUCCESS

#include "main/game.hpp" // for Game

int main()
{
    Game game {};

    game.run();

    return EXIT_SUCCESS;
}