#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "components/component.hpp"

namespace tile
{
    class Cursor : public Component
    {
      public:
        enum Type
        {
            Outline,
            Full
        };

        sf::RectangleShape m_shape;
        Type               m_type;

      public:
        Cursor( Type type );
        virtual ~Cursor() = default;

      private:
        void render ( RenderContext context ) const override;

      public:
        void hide ();
        void show_at_position ( math::Vector2F position );

      private:
        void set_color ( sf::Color color );
    };
}  // namespace tile