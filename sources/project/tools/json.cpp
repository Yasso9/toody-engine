#include "json.hpp"

namespace jsonaddon
{
    std::vector<std::vector<std::vector<unsigned int>>> decode_array(
        json const & json3dArray )
    {
        std::vector<std::vector<std::vector<unsigned int>>> real3dArray {};

        for ( auto const & json2dArray : json3dArray )
        {
            std::vector<std::vector<unsigned int>> real2dArray {};

            for ( auto const & json1dArray : json2dArray )
            {
                std::vector<unsigned int> real1dArray {};

                for ( auto const & element : json1dArray )
                {
                    real1dArray.push_back( element );
                }

                real2dArray.push_back( real1dArray );
            }

            real3dArray.push_back( real2dArray );
        }

        return real3dArray;
    }
} // namespace jsonaddon