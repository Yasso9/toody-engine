#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "application/components/component.hpp"
#include "graphics2D/tile/position.hpp"

namespace tile
{
    class Map;

    class MouseCursor : public Component
    {
      public:
        enum Type
        {
            Outline,
            Full
        };

        sf::RectangleShape                      m_shape;
        Type                                    m_type;
        std::function< void( tile::Position ) > m_on_click;

      public:
        MouseCursor( Type type );

        void manual_update ( UpdateContext &   context,
                             tile::Map const & tilemap );

      private:
        void render ( RenderContext & context ) const override;

      public:
        void hide ();
        void show ();
        void set_position ( math::Vector2F position );
        void on_click ( std::function< void( tile::Position ) > callback );

      private:
        void set_color ( sf::Color color );
    };
}  // namespace tile
