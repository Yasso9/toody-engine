#pragma once

#include <optional>

#include "SFML/Graphics/Sprite.hpp"
#include "components/component.hpp"  // for Component
#include "graphics2D/color.hpp"
#include "graphics2D/tile/set.hpp"  // for Tileset

namespace tile
{
    class Selector : public Component
    {
        /// @brief Table of tiles where we can select our tile
        tile::Set m_tileset;
        /// @brief posisition of the tile chosen. Is optional because at the
        /// beginning we can have no tile choose
        std::optional< tile::Position > m_tileSelected;

        /// @brief Color of the grid
        Color m_gridColor;

      public:
        Selector();
        virtual ~Selector() = default;

        void update ( UpdateContext & context ) override;

        tile::Set const &               get_tileset () const;
        std::optional< tile::Position > get_tile_selected () const;
    };
}  // namespace tile
