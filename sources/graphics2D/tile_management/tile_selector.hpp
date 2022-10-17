#pragma once

#include <optional>

#include "graphics2D/color.hpp"
#include "graphics2D/component.hpp"                // for Component2D
#include "graphics2D/tile_management/tileset.hpp"  // for Tileset

struct ImDrawList;

namespace tile
{
    class Selector : public Component2D
    {
        /// @brief Table of tiles where we can select our tile
        Tileset                       m_tileset;
        /// @brief index of the tile chosen. Is optional because at the
        /// beginning we can have no tile choose
        std::optional< unsigned int > m_tileSelected;

        /// @brief Should we show the grid on the tileset
        bool  m_isGridEnabled;
        /// @brief Color of the grid
        Color m_gridColor;

      public:
        Selector();
        virtual ~Selector() = default;

        Tileset const &               get_tileset () const;
        std::optional< unsigned int > get_tile_selected () const;

        void update_before ( float deltaTime ) override;

      private:
        void draw_grid ( ImDrawList & drawList );
        void update_selection ( ImDrawList & drawList );
    };

}  // namespace tile