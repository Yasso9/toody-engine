#pragma once

#include "states/state.hpp"

#include "output_info/information.hpp"
#include "tilemap/editor.hpp"
#include "tilemap/tilemap.hpp"
#include "tilemap/tileset.hpp"

class EditorState final : public State
{
  public:
    EditorState();
    ~EditorState() = default;

    void extra_events() override;

    void update() override;
    void render() const override;

  private:
    // EditorMap m_tilemap;
    // Tileset m_tileset;
    TileMapEditor m_tilemapEditor;

    sf::View m_view;

    void init_map();

    void keyboard_pressed( sf::Event event ) override;
    void mouse_scroll( float const & deltaScroll ) override;
};
