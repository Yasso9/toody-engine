#pragma once

#include "graphics2D/component.hpp" // for Component2D
#include "graphics2D/tileset.hpp"   // for Tileset

struct ImDrawList;

class TileSelector : public Component2D
{
    Tileset m_tileset;

    int m_tileSelected;

    bool m_isGridEnabled;
    float m_gridColorTable[4];

  public:
    TileSelector();
    virtual ~TileSelector() = default;

    Tileset const & get_tileset() const;

    int get_tile_selected() const;

    void update_before( float deltaTime ) override;

  private:
    void update_grid( ImDrawList & drawList );
    void update_selection( ImDrawList & drawList );
};