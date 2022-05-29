#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace jsonaddon
{
    std::vector< std::vector< std::vector< int > > > decode_array(
        json const & json3dArray );
} // namespace jsonaddon