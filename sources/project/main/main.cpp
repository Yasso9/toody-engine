#include <project/main/game.hpp>

int main()
{
    Game game {};

    std::cout << "Game Opened"s << std::endl;

    game.run();

    std::cout << "Game Closed"s << std::endl;

    return EXIT_SUCCESS;
}