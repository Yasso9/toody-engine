#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "tools/global_variable.hpp"

namespace tile
{
    /// @todo Set it as a component
    class Cursor
    {
      public:
        sf::RectangleShape m_shape;

        Cursor();
        virtual ~Cursor() = default;

      private:
    };
}  // namespace tile