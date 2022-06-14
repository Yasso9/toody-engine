#include "main/game.hpp"

#include "tools/databases.hpp"
#include "tools/serialization.hpp"

int main()
{
    Game game {};

    game.run();

    return EXIT_SUCCESS;

    // test_serializer();

    // db::test_database();

    return 0;
}