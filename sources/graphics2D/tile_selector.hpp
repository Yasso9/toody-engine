#pragma once

#include "graphics2D/tileset.hpp"

class TileSelector
{
    Tileset m_tileset;

    int m_tileSelected;

    bool m_isGridEnabled;
    float m_gridColorTable[4];

    bool m_isLeftButtonPressed;
    math::Vector2F m_mousePosition;

  public:
    TileSelector();
    virtual ~TileSelector() = default;

    Tileset const & get_tileset() const;

    int get_tile_selected() const;

    void process_events();
    void update();

  private:
    void update_grid( ImDrawList & drawList );
    void update_selection( ImDrawList & drawList );
};