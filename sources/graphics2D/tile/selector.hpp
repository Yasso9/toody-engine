#pragma once

#include <optional>

#include "SFML/Graphics/Sprite.hpp"
#include "application/components/component.hpp"  // for Component
#include "graphics2D/color.hpp"
#include "graphics2D/tile/set.hpp"  // for Tileset

namespace tile
{
    class Selector : public Component
    {
        struct Show
        {
            bool grid;
            bool scrollbar;
        };

        struct SubTileset
        {
            tile::Size     size;
            tile::Position firstTile;

            SubTileset( tile::Set const & tileset, tile::Size const & maxSize )
              : size { math::min( tileset.get_size().tile(), maxSize.tile() ),
                       tile::Size::Tile },
                firstTile { { 0, 0 }, size, tile::Position::Tile }
            {}
        };

        tile::Set  m_tileset;
        SubTileset m_subTileset;
        // position of the tile chosen. Is optional because at the beginning we
        // can have no tile choose
        std::optional< tile::Position > m_tileSelected;

        Color m_gridColor;

        Show m_show;

        DebugWindow m_debugWindow;

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

        void update_tileset ();
    };
}  // namespace tile
