#pragma once

#include <optional>

#include "component/component.hpp"  // for Component
#include "graphics2D/color.hpp"
#include "graphics2D/tile_management/tileset.hpp"  // for Tileset

struct ImDrawList;

namespace tile
{
    class Selector : public Component
    {
        /// @brief Table of tiles where we can select our tile
        Tileset                         m_tileset;
        /// @brief posisition of the tile chosen. Is optional because at the
        /// beginning we can have no tile choose
        std::optional< tile::Position > m_tileSelected;

        /// @brief Color of the grid
        Color m_gridColor;

      public:
        Selector();
        virtual ~Selector() = default;

        void update ( float deltaTime ) override;

        Tileset const &                 get_tileset () const;
        std::optional< tile::Position > get_tile_selected () const;

      private:
        void update_selection ( ImDrawList & drawList );
    };
}  // namespace tile