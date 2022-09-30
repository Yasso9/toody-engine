#pragma once

#include <optional>

#include "graphics2D/component.hpp"                // for Component2D
#include "graphics2D/tile_management/tileset.hpp"  // for Tileset

struct ImDrawList;

class TileSelector : public Component2D
{
    /// @brief Table of tiles where we can select our tile
    Tileset                       m_tileset;
    /// @brief index of the tile chosen.
    std::optional< unsigned int > m_tileSelected;

    bool  m_isGridEnabled;
    float m_gridColorTable[4];

  public:
    TileSelector();
    virtual ~TileSelector() = default;

    Tileset const &               get_tileset () const;
    std::optional< unsigned int > get_tile_selected () const;

    void update_before ( float deltaTime ) override;

  private:
    void update_grid ( ImDrawList & drawList );
    void update_selection ( ImDrawList & drawList );
};