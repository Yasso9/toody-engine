#include <project/main/game.hpp>

int main()
{
    Game game {};

    try
    {
        game.run();
    }
    catch ( FileError const & exception )
    {
        std::cout << "FileError : " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}