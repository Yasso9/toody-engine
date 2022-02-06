#pragma once

// To ignore some warning caused by the include of Sqlite
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"

extern "C"
{
#include <SQLITE/sqlite3.h>
}

#pragma GCC diagnostic pop

#include "tools/json.hpp"

#include <memory>

// To mix the use of unique pointer and sqlite3 database
struct sqlite3_deleter
{
    void operator()( sqlite3 * sql ) const { sqlite3_close( sql ); }
};

// TYPO mettre ce commentaire dans un autre fichier (readme peut être)
// t == type
using t_uniqueSqlitePtr = std::unique_ptr<sqlite3, sqlite3_deleter>;

namespace database
{
    /// @brief Send a request to the game database in Sqlite format and get the result in json
    json request( std::string const & request );
} // namespace database