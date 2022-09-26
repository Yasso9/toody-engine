#pragma once

#include <iostream>
#include <string>  // for string

#include "tools/serialization.hpp"  // for Unserializer

namespace db
{
    /// @todo don't take a string but take parameter containing the tablename,
    /// array of arguments, etc ...
    Unserializer request ( std::string const & request );

    class Table
    {
        std::string m_name;

      public:
        Table( std::string name ) : m_name { name }
        {
            std::ostringstream requestStream {};
            requestStream
                << "SELECT name FROM sqlite_master WHERE type = 'table' AND "
                   "name = '"
                << m_name << "';";
            std::cout << requestStream.str() << std::endl;
            std::cout << db::request( requestStream.str() ).get_content()
                      << std::endl;
        }

        ~Table() = default;

        template< typename Type >
        Type select ( std::string variableName )
        {
            std::ostringstream requestStream {};
            requestStream << "SELECT " << m_name << " FROM " << variableName
                          << ";";
            return db::request( requestStream.str() ).to_value< Type >();
        }

        template< typename Type >
        void insert ( std::string variableName, Type const & value )
        {
            std::ostringstream requestStream {};
            requestStream << "INSERT INTO " << m_name << " (" << variableName
                          << ")"
                          << "VALUES('" << Serializer { value }.to_string()
                          << "');";
        }
    };

    [[maybe_unused]] void test_table ();
    [[maybe_unused]] void test_database ();
}  // namespace db