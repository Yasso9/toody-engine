#include "databases.hpp"

#include <memory>

// To ignore some warning caused by the include of Sqlite
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"

extern "C"
{
#include <SQLITE/sqlite3.h>
}

#pragma GCC diagnostic pop

#include "tools/exceptions.hpp"
#include "tools/string.hpp"

#include "tools/resources.hpp"

// To mix the use of unique pointer and sqlite3 database
struct sqlite3_deleter
{
    void operator()( sqlite3 * sql ) const { sqlite3_close( sql ); }
};

using T_UniqueSqlitePtr = std::unique_ptr<sqlite3, sqlite3_deleter>;

/// @brief Initialize the database
static T_UniqueSqlitePtr make_sqlite();

// Json Array of all the result requested
static json gf_resultRequested {};

namespace db
{
    json request( std::string const & request )
    {
        T_UniqueSqlitePtr database { ::make_sqlite() };

        char * requestErrorMessage { const_cast<char *>( "" ) };

        // Reset at every request
        gf_resultRequested = json {};

        // To get back the result of the request
        auto cb_result_request {
            []( void * /* data */, int argc, char ** argv, char ** azColName )
            {
                // TYPO data inutile ? Voir à quoi ça sert
                // std::cout << "Data : " << data << std::endl;

                // Map containing pair of table property and its json value
                std::map<std::string, json> singleResultMap {};

                // TYPO
                // for(auto i : argc)
                for ( int i { 0 }; i < argc; ++i )
                {
                    singleResultMap.insert(
                        std::make_pair( azColName[i],
                                        argv[i] ? argv[i] : "NULL" ) );
                }

                gf_resultRequested.push_back( singleResultMap );

                return 0;
            }
        };

        int const requestResultState { sqlite3_exec( database.get(),
                                                     request.c_str(),
                                                     cb_result_request,
                                                     0,
                                                     &requestErrorMessage ) };

        if ( requestResultState != SQLITE_OK )
        {
            sqlite3_free( requestErrorMessage );
            throw DatabaseException { too::get_databases_path(),
                                      requestErrorMessage };
        }

        return gf_resultRequested;
    }
} // namespace db

static T_UniqueSqlitePtr make_sqlite()
{
    sqlite3 * database { nullptr };

    if ( sqlite3_open( too::get_databases_path().c_str(), &database ) )
    {
        // Something bad is happenning
        throw DatabaseException { too::get_databases_path(),
                                  "Can't open database - "s
                                      + sqlite3_errmsg( database ) };
    }

    return T_UniqueSqlitePtr( database );
}

// Initialisation de la database

// TYPO mettre tous les databases dans la bonne place
// database::request( R"(
// DROP TABLE IF EXISTS tilemap;
// CREATE TABLE tilemap(
//     table_tilemap TEXT NOT NULL
// );
// )"s );

// std::vector<std::vector<std::vector<unsigned int>>> tripleArray {
//     {
//         { 5, 3, 4, 4, 2, 5 },
//         { 5, 3, 4, 4, 2, 5 },
//         { 5, 3, 4, 4, 2, 5 },
//     },
//     {
//         { 5, 3, 4, 4, 2, 5 },
//         { 5, 3, 4, 4, 2, 5 },
//         { 5, 3, 4, 4, 2, 5 },
//     }
// };

// json jsonTest {};
// jsonTest["array"] = tripleArray;

// database::request( R"(
// INSERT INTO tilemap( table_tilemap )
// VALUES( ")"s + jsonTest["array"].dump()
//                    +
//                    R"(" );
// INSERT INTO tilemap( table_tilemap )
// VALUES( ")"s + jsonTest["array"].dump()
//                    +
//                    R"(" );
// INSERT INTO tilemap( table_tilemap )
// VALUES( ")"s + jsonTest["array"].dump()
//                    +
//                    R"(" );
// )"s );