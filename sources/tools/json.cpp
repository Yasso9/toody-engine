#include "json.hpp"

#include "iostream"

namespace jsonaddon
{
    std::vector< std::vector< std::vector< int > > > decode_array(
        json const & json3dArray )
    {
        std::vector< std::vector< std::vector< int > > > real3dArray {};

        /// @brief d'ou sort cette array ?
        for ( auto const & idkArray : json3dArray )
        {
            for ( auto const & json2dArray : idkArray )
            {
                std::cout << "json2dArray" << json2dArray << std::endl;
                std::vector< std::vector< int > > real2dArray {};

                for ( auto const & json1dArray : json2dArray )
                {
                    std::cout << "json1dArray" << json1dArray << std::endl;
                    std::vector< int > real1dArray {};

                    for ( auto const & element : json1dArray )
                    {
                        std::cout << "element" << element << std::endl;
                        real1dArray.push_back( element );
                    }

                    real2dArray.push_back( real1dArray );
                }

                real3dArray.push_back( real2dArray );
            }
        }

        return real3dArray;
    }
} // namespace jsonaddon