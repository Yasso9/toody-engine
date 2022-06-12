#pragma once

#include "states/state.hpp"

#include "entity/player.hpp"
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
    sf::View m_view;
    TileMap m_tilemap;

    Player m_player;

    bool m_showDemoWindow;
    bool m_showDebugOptions;
    bool m_showEditorOverlay;
    bool m_handlePlayer;

    void init_map();

    void keyboard_pressed( sf::Event event ) override;
    void mouse_scroll( float const & deltaScroll ) override;
};
