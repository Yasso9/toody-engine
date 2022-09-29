#pragma once

#include <iostream>
#include <string>  // for string

#include "tools/exceptions.hpp"
#include "tools/path.hpp"
#include "tools/serialization.hpp"  // for Unserializer

namespace db
{
    /// @todo don't take a string but take parameter containing the tablename,
    /// array of arguments, etc ...
    Chunk request ( std::string const & request );

    bool is_table_created ( std::string tableName );

    class Table
    {
        std::string m_name;

      public:
        Table( std::string name );
        ~Table() = default;

        bool has_attribute ( std::string attribute );

        template< typename Type >
        Type select ( std::string attribute );

        template< typename Type >
        void insert ( std::string variableName, Type const & value );
    };

    template< typename Type >
    Type Table::select( std::string attribute )
    {
        std::ostringstream requestStream {};
        requestStream << "SELECT " << attribute << " FROM " << m_name << ";";
        return db::request( requestStream.str() ).to_value< Type >();
    }

    template< typename Type >
    void Table::insert( std::string variableName, Type const & value )
    {
        std::ostringstream requestStream {};
        requestStream << "INSERT INTO " << m_name << " (" << variableName << ")"
                      << "VALUES('" << serialize( value ).to_string() << "');";
    }

    [[maybe_unused]] void test_table ();
    [[maybe_unused]] void test_database ();
}  // namespace db
