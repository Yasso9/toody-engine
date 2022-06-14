#pragma once

#include <string>

#include "tools/serialization.hpp"

namespace db
{
    /// @todo retourner un json en créer un nouveau tableau donc on est obligé d'avoir 0
    /// @todo don't take a string but take parameter containing the tablename, array of arguments, etc ...
    /// @brief Send a request to the game database in Sqlite format and get the result in json
    Unserializer request( std::string const & request );

    [[maybe_unused]] void test_database();
} // namespace db