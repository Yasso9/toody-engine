#pragma once

#include <iostream>
#include <string>  // for string

#include "tools/exceptions.hpp"
#include "tools/path.hpp"
#include "tools/serialization.hpp"  // for Unserializer

namespace db
{
    Chunk request ( std::string const & request, bool checkError = true );

    bool is_table_created ( std::string tableName );

    class Table
    {
        std::string m_name;

      public:
        Table( std::string name );
        ~Table() = default;

        bool has_attribute ( std::string attribute ) const;

        template< typename Type >
        Type select ( std::string attribute, bool checkError = true ) const;

        template< typename Type >
        void insert ( std::string variableName, Type const & value ) const;
    };

    template< typename Type >
    Type Table::select( std::string attribute, bool checkError ) const
    {
        std::ostringstream requestStream {};
        requestStream << "SELECT " << attribute << " FROM " << m_name << ";";
        return db::request( requestStream.str(), checkError )
            .to_value< Type >();
    }

    template< typename Type >
    void Table::insert( std::string variableName, Type const & value ) const
    {
        std::ostringstream requestStream {};
        requestStream << "INSERT INTO " << m_name << " (" << variableName << ")"
                      << "VALUES('" << serialize( value ).to_string() << "');";
    }

    [[maybe_unused]] void test_table ();
    [[maybe_unused]] void test_database ();
}  // namespace db
