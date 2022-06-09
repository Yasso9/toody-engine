#pragma once

#include "tools/json.hpp"

namespace db
{
    /// @todo retourner un json en créer un nouveau tableau donc on est obligé d'avoir 0
    /// @brief Send a request to the game database in Sqlite format and get the result in json
    json request( std::string const & request );
} // namespace db