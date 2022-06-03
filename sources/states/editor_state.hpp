#pragma once

#include "states/state.hpp"

#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tilemap.hpp"
#include "graphics2D/tileset.hpp"

class EditorState final : public State
{
  public:
    EditorState();
    ~EditorState() = default;

    void extra_events() override;

    void update() override;
    void render() const override;

  private:
    TileSelector m_tileSelector;
    TileMap m_tilemap;

    sf::View m_view;

    bool m_showDemoWindow;
    bool m_showTilemapEditor;
    bool m_showDebugOptions;

    void init_map();

    void keyboard_pressed( sf::Event event ) override;
    void mouse_scroll( float const & deltaScroll ) override;
};
