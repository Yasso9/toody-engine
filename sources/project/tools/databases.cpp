#include "databases.hpp"

#include <project/tools/exceptions.hpp>
#include <project/tools/string.hpp>

// TYPO mettre ce commentaire dans un autre fichier (readme peut être)
// tips variable names : 'gf_' == global in this file

// TYPO demander si c'est bien de mettre ce namespace ici ou alors
// il faut le mettre dans namespace database pour correctement le cacher
namespace
{
    std::string const gf_databasePath { "program_database.s3db"s };

    /// @brief Initialize the database
    t_uniqueSqlitePtr make_sqlite()
    {
        sqlite3 * database { nullptr };

        if ( sqlite3_open( ::gf_databasePath.c_str(), &database ) )
        {
            // Something bad is happenning
            throw DatabaseException { ::gf_databasePath,
                                      "Can't open database - "s
                                          + sqlite3_errmsg( database ) };
        }

        return t_uniqueSqlitePtr( database );
    }

    // Json Array of all the result requested
    json gf_resultRequested {};
} // namespace

namespace database
{
    json request( std::string const & request )
    {
        t_uniqueSqlitePtr database { ::make_sqlite() };

        char * requestErrorMessage { const_cast<char *>( "" ) };

        // Reset at every request
        ::gf_resultRequested = json {};

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

                ::gf_resultRequested.push_back( singleResultMap );

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
            throw DatabaseException { ::gf_databasePath, requestErrorMessage };
        }

        return ::gf_resultRequested;
    }
} // namespace database