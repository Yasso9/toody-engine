#include "databases.hpp"

#include <iostream>
#include <memory>
#include <sstream>

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
#include "tools/serialization.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

static std::string s_requestResult {};
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

        s_requestResult = value;
    }

    return 0;
}

/// @todo create a singleton and initialize sqlite3_open
namespace db
{
    Unserializer request( std::string const & request )
    {
        sqlite3 * database { nullptr };
        if ( sqlite3_open( g_databasePath.c_str(), &database ) )
        {
            throw Exception::Database { g_databasePath,
                                        "Can't open database - "s
                                            + sqlite3_errmsg( database ) };
        }

        // Reset before every request
        s_requestResult.clear();

        char * requestErrorMessage { const_cast< char * >( "" ) };
        int result = sqlite3_exec( database,
                                   request.c_str(),
                                   callback,
                                   0,
                                   &requestErrorMessage );

        if ( result != 0 )
        {
            sqlite3_free( requestErrorMessage );
            throw Exception::Database { g_databasePath, requestErrorMessage };
        }

        sqlite3_close( database );

        return Unserializer { s_requestResult };
    }

    void test_database()
    {
        // Initialisation de la database

        /// @todo mettre tous les databases dans la bonne place
        Unserializer initRequest = db::request( "DROP TABLE IF EXISTS tilemap;"
                                                "CREATE TABLE tilemap ("
                                                "tile_table TEXT NOT NULL"
                                                ");" );
        std::cout << "initRequest : |" << initRequest.get_content() << "|"
                  << std::endl;

        std::vector< std::vector< std::vector< unsigned int > > > tripleArray {
            {{ 0 }, { 2 }},
            {{ 2 }, { 0 }}
        };

        std::cout << "tripleArray : |" << tripleArray << "|" << std::endl;
        std::cout << "tripleArray serialized : |"
                  << Serializer( tripleArray ).to_string() << "|" << std::endl;

        Unserializer insertionRequest =
            db::request( "INSERT INTO tilemap (tile_table)"
                         "VALUES('"
                         + Serializer { tripleArray }.to_string() + "');" );

        std::cout << "insertionRequest : |" << insertionRequest.get_content()
                  << "|" << std::endl;

        Unserializer selectionRequest { db::request(
            "SELECT tile_table FROM tilemap;" ) };

        std::cout << "selectionRequest content : |"
                  << selectionRequest.get_content() << "|" << std::endl;
        std::cout << "selectionRequest unserialized: |"
                  << selectionRequest.to_value< decltype( tripleArray ) >()
                  << "|" << std::endl;
    }
} // namespace db