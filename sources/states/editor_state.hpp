#pragma once

#include <filesystem>
#include <map>

#include "states/state.hpp"

#include "entity/object.hpp"
#include "entity/player.hpp"
#include "graphics2D/image.hpp"
#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tilemap.hpp"
#include "graphics2D/tileset.hpp"
#include "graphics2D/view.hpp"

class EditorState final : public State
{
    enum class E_WindowKey
    {
        DemoWindow = 0,
        DebugOptions,
        EditorOverlay,
        Collision,
        PlayerHandling,
    };
    std::map< E_WindowKey, bool > m_showWindow;

    View m_view;
    TileMap m_tilemap;
    // ImageMap m_imageMap;

    Player m_player;

    T_CollisionMap m_collisionMap;
    Entity2D m_greenEntity;

    /// @todo Create a class events who take paramters like mouse position and mouse click that is updated automatically
    math::Vector2I m_mousePosition;

  public:
    EditorState();

    void mouse_moved( sf::Event event ) override;
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
