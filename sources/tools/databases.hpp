#pragma once

#include <string>

#include "tools/serialization.hpp"

namespace db
{
    /// @todo don't take a string but take parameter containing the tablename, array of arguments, etc ...
    Unserializer request( std::string const & request );

    [[maybe_unused]] void test_database();
} // namespace db