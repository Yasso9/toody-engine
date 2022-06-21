#include "main/game.hpp"

#include <iostream>

int main()
{
    std::cout << "GAME RUN" << std::endl;

    Game game {};

    game.run();

    std::cout << "GAME END" << std::endl;

    return EXIT_SUCCESS;
}