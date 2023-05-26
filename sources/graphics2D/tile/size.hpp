#pragma once

#include "maths/vector2.hpp"

namespace tile
{
    class Size
    {
        /// @brief Size in tile value
        math::Vector2U m_size;

      public:
        enum Type
        {
            Tile = 0,
            Pixel
        };

        Size( math::Vector2U size, Type type );

        unsigned int value () const;
        /// @brief get size in tile value
        math::Vector2U tile () const;
        /// @brief get size in pixel value
        math::Vector2U pixel () const;

        void set_value ( math::Vector2U position, Type type );
    };
}  // namespace tile
