#pragma once

#include <optional>

#include "SFML/Graphics/Sprite.hpp"
#include "application/components/component.hpp"  // for Component
#include "graphics2D/color.hpp"
#include "graphics2D/tile/set.hpp"   // for Tileset

namespace tile
{
    class Selector : public Component
    {
        struct Show
        {
            bool debug;
            bool grid;
            bool scrollbar;
        };

        tile::Set      m_tileset;
        tile::Position m_subTilesetPosition;
        tile::Size     m_subTilesetSize;
        // position of the tile chosen. Is optional because at the beginning we
        // can have no tile choose
        std::optional< tile::Position > m_tileSelected;

        Color m_gridColor;

        Show m_show;

      public:
        Selector();

        void update ( UpdateContext & context ) override;

        tile::Set const &               get_tileset () const;
        std::optional< tile::Position > get_tile_selected () const;

      private:
        void update_settings ( UpdateContext & context );
        void update_grid ( UpdateContext & context );
        void update_scroll ( UpdateContext & context );
        void update_selection ( UpdateContext & context );
        void update_informations ( UpdateContext & context );

        void show_tileset ();
    };
}  // namespace tile
