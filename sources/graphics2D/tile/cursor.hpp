#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

namespace tile
{
    /// @todo Set it as a component
    class Cursor
    {
      public:
        sf::RectangleShape shape;

        Cursor();
        virtual ~Cursor() = default;

      private:
    };
}  // namespace tile