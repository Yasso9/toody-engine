#include "databases.hpp"

#include <iostream>  // for operator<<, basic_ostream, endl
#include <vector>    // for vector

extern "C"
{
#include <SQLITE/sqlite3.h>  // for sqlite3_close, sqlite3_errmsg
}

#include "tools/exceptions.hpp"     // for Database
#include "tools/path.hpp"           // for get_file_str, E_File, E_File::Dat...
#include "tools/serialization.hpp"  // for Unserializer, Serializer
#include "tools/serialization.tpp"  // for operator<<, Serializer::Serialize...

static std::string s_requestResult {};

static int callback (
    void * /* data */, int argc, char ** argv, char ** azColName )
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
namespace database
{
    Chunk request ( std::string const & request, bool checkError )
    {
        std::string const databasePath {
            path::get_file_str( path::E_File::Database ) };

        sqlite3 * database { nullptr };
        if ( sqlite3_open( databasePath.c_str(), &database ) )
        {
            throw exception::Database {
                databasePath,
                "Can't open database - "s + sqlite3_errmsg( database ) };
        }

        // Reset before every request
        s_requestResult.clear();

        char * requestErrorMessage { const_cast< char * >( "" ) };
        int    result = sqlite3_exec(
               database, request.c_str(), callback, 0, &requestErrorMessage );

        if ( result != 0 )
        {
            sqlite3_free( requestErrorMessage );
            if ( checkError )
            {
                std::cerr << "ERROR Database - file " << databasePath << " - "
                          << requestErrorMessage << std::endl;
            }
        }

        sqlite3_close( database );

        return Chunk { s_requestResult };
    }

    bool is_table_created ( std::string tableName )
    {
        std::ostringstream requestStream {};
        requestStream
            << "SELECT name FROM sqlite_master WHERE type = 'table' AND "
               "name = '"
            << tableName << "';";

        return database::request( requestStream.str() ).to_string()
               == tableName;
    }

    /* ********************************************************************
    ***************************** TABLE CLASS *****************************
    ******************************************************************** */

    Table::Table( std::string name ) : m_name { name }
    {
        if ( ! is_table_created( name ) )
        {
            throw exception::Database {
                path::get_file_str( path::E_File::Database ),
                "Table " + name + " not found" };
        }
    }

    bool Table::has_attribute( std::string attribute ) const
    {
        return this->select< std::string >( attribute, false ) != "";
    }

    /* ********************************************************************
    ******************************** TESTS ********************************
    ******************************************************************** */

    void test_table ()
    {
        // std::cout << std::boolalpha;
        std::cout << "Is Table 'tilemap' created ? "
                  << is_table_created( "tilemap" ) << std::endl;
        std::cout << "Is Table 'dghsjkdh' created ? "
                  << is_table_created( "dghsjkdh" ) << std::endl;

        Table table { "tilemap" };

        std::cout << "Attribut 'tile_table' content : '"
                  << table.select< std::string >( "tile_table" ) << "'"
                  << std::endl;
        std::cout << "Attribut 'ozkeokd' content : '"
                  << table.select< std::string >( "ozkeokd", false ) << "'"
                  << std::endl;

        std::cout << "Is 'tile_table' attribute of 'tilemap' ? "
                  << table.has_attribute( "tile_table" ) << std::endl;
        std::cout << "Is 'ozkeokd' attribute of 'tilemap' ? "
                  << table.has_attribute( "ozkeokd" ) << std::endl;
    }

    void test_database ()
    {
        // Initialisation de la database

        Chunk initRequest = database::request(
            "DROP TABLE IF EXISTS tilemap;"
            "CREATE TABLE tilemap ("
            "tile_table TEXT NOT NULL"
            ");" );
        std::cout << "initRequest : |" << initRequest.to_string() << "|"
                  << std::endl;

        std::vector< std::vector< std::vector< unsigned int > > > tripleArray {
            {{ 0 }, { 2 }},
            {{ 2 }, { 0 }}
        };

        std::cout << "tripleArray : |" << tripleArray << "|" << std::endl;
        std::cout << "tripleArray serialized : |"
                  << serialize( tripleArray ).to_string() << "|" << std::endl;

        Chunk insertionRequest = database::request(
            "INSERT INTO tilemap (tile_table)"
            "VALUES('"
            + serialize( tripleArray ).to_string() + "');" );

        std::cout << "insertionRequest : |" << insertionRequest.to_string()
                  << "|" << std::endl;

        Chunk selectionRequest {
            database::request( "SELECT tile_table FROM tilemap;" ) };

        std::cout << "selectionRequest content : |"
                  << selectionRequest.to_string() << "|" << std::endl;
        std::cout << "selectionRequest unserialized: |"
                  << selectionRequest.to_value< decltype( tripleArray ) >()
                  << "|" << std::endl;
    }
}  // namespace database