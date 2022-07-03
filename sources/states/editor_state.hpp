#pragma once

#include "states/state.hpp"

#include "entity/object.hpp"
#include "entity/player.hpp"
#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tilemap.hpp"
#include "graphics2D/tileset.hpp"
#include "graphics2D/view.hpp"

class EditorState final : public State
{
    View m_view;
    TileMap m_tilemap;

    Player m_player;

    T_CollisionMap m_collisionMap;
    Entity2D m_greenEntity;

    /// @todo create a map and enum as key to handles all the boolean variable show demo
    bool m_showDemoWindow;
    bool m_showDebugOptions;
    bool m_showEditorOverlay;
    /// @todo faire de player un std::optionnal, pour que lorsuqu'il sera mis à  faux, le player ne pourra pas bouger
    bool m_handlePlayer;
    bool m_showCollisionWindow;

    /// @todo Create a class events who take paramters like mouse position and mouse click that is updated automatically
    math::Vector2I m_mousePosition;

  public:
    EditorState();

    void extra_events() override;

    void update() override;
    void render() const override;

  private:
    void init_map();

    void keyboard_pressed( sf::Event event ) override;
    void mouse_scroll( float const & deltaScroll ) override;

    void update_toolbar();
    void update_debug_window();
    void update_collision_window();
    void update_overlay();
};
