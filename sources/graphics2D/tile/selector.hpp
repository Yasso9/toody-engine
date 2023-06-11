#pragma once

#include <memory>    // for shared_ptr
#include <optional>  // for optional

#include "SFML/Graphics/Sprite.hpp"
#include "application/components/component.hpp"  // for Component
#include "graphics2D/color.hpp"                  // for Color
#include "graphics2D/tile/set.hpp"               // for Tileset
#include "imgui/debug_window.hpp"                // for DebugWindow
#include "imgui/sub_window.hpp"                  // for SubWindow

namespace tile
{
    class SubTileset : public Grid
    {
        math::Vector2F m_position;
        tile::Size     m_size;
        tile::CellPos  m_firstTile;

      public:
        SubTileset( tile::Set const & tileset, tile::Size const & maxSize );

        math::Vector2F get_position () const override;
        void           set_position ( math::Vector2F const & pos ) override;
        tile::Size     get_size () const override;

        tile::CellPos first_tile () const;
        void          set_first_tile ( math::Vector2F const & scroll );

        // ImGui slider to modify the size
        void size_slider ();
    };

    class Selector : public SubWindow
    {
        struct Show
        {
            bool grid;
        };

        std::shared_ptr< tile::Set const > m_tileset;
        SubTileset                         m_subTileset;
        // position of the tile chosen. Is optional because at the beginning we
        // can have no tile choose
        std::optional< tile::CellPos > m_tileSelected;

        Color m_gridColor;
        Show  m_showElements;

        DebugWindow m_debugWindow;

      public:
        Selector( std::shared_ptr< tile::Set const > tileset );

        void update_window ( UpdateContext & context ) override;

        void set_tileset ( std::shared_ptr< tile::Set const > tileset );
        std::optional< tile::CellPos > get_tile_selected () const;

      private:
        void update_config ( UpdateContext & context );
        void update_scroll ( UpdateContext & context );
        void update_image ( UpdateContext & context );
        void update_grid ( UpdateContext & context );
        void update_selection ( UpdateContext & context );
        void update_informations ( UpdateContext & context );
    };
}  // namespace tile
