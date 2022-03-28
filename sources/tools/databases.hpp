#pragma once

#include "tools/json.hpp"

namespace db
{
    /// @brief Send a request to the game database in Sqlite format and get the result in json
    json request( std::string const & request );
} // namespace db