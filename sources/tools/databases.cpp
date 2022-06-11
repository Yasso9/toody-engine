#include "databases.hpp"

#include <iostream>
#include <memory>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#if defined( __clang__ )
    #pragma GCC diagnostic ignored "-Wlanguage-extension-token"
#endif
extern "C"
{
#include <SQLITE/sqlite3.h>
}
#pragma GCC diagnostic pop

#include "tools/exceptions.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

// Json Array of all the result requested
static json s_requestResult {};
static std::string const g_databasePath { tools::get_path::databases()
                                          + "game.db" };

static int callback( void * /* data */, int argc, char ** argv,
                     char ** azColName )
{
    for ( unsigned int i { 0u }; i < static_cast< unsigned int >( argc ); ++i )
    {
        // std::cout << "i : " << i << std::endl;
        // std::cout << "azColName[i] : " << azColName[i] << std::endl;
        // std::cout << "argv[i] : " << argv[i] << std::endl;

        std::string const name { azColName[i] };
        std::string const value { argv[i] ? argv[i] : "NULL" };

        s_requestResult[name] = value;
    }

    return 0;
}

/// @todo create a singleton and initialize sqlite3_open
namespace db
{
    json request( std::string const & request )
    {
        sqlite3 * database { nullptr };
        if ( sqlite3_open( g_databasePath.c_str(), &database ) )
        {
            throw Exception::Database { g_databasePath,
                                        "Can't open database - "s
                                            + sqlite3_errmsg( database ) };
        }

        // Reset at every request
        s_requestResult = json {};

        char * requestErrorMessage { const_cast< char * >( "" ) };
        int result = sqlite3_exec( database,
                                   request.c_str(),
                                   callback,
                                   0,
                                   &requestErrorMessage );

        if ( result != 0 )
        {
            throw Exception::Database { g_databasePath, requestErrorMessage };
            sqlite3_free( requestErrorMessage );
        }

        sqlite3_close( database );

        /// @todo always return an array : solution - put the return in a string instead of a json
        return s_requestResult;
    }
} // namespace db

[[maybe_unused]] static void test()
{
    // Initialisation de la database

    /// @todo mettre tous les databases dans la bonne place
    db::request( "DROP TABLE IF EXISTS tilemap;"
                 "CREATE TABLE tilemap ("
                 "tile_table TEXT NOT NULL"
                 ");" );
    std::vector< std::vector< std::vector< unsigned int > > > tripleArray {
        {{ 0 }, { 2 }},
        {{ 2 }, { 0 }}
    };

    json jsonArray {};
    jsonArray = tripleArray;

    std::cout << "dump : '" << jsonArray.dump() << "'" << std::endl;

    json insertionRequest = db::request( "INSERT INTO tilemap (tile_table)"
                                         "VALUES('"
                                         + jsonArray.dump() + "');" );

    std::cout << "insertionRequest " << insertionRequest << std::endl;

    json const requestValue { db::request(
        "SELECT tile_table FROM tilemap;" ) };

    std::cout << "requestValue " << requestValue << std::endl;
    std::cout << "requestValue " << requestValue[0] << std::endl;
}