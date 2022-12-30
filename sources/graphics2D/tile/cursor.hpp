#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "component/component.hpp"

namespace tile
{
    /// @todo have rounded corner
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
        void render ( Render & render ) const override;

      public:
        void hide ();
        void show_at_position ( math::Vector2F position );

      private:
        void set_color ( sf::Color color );
    };
}  // namespace tile