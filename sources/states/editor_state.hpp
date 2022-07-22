#pragma once

#include <filesystem>
#include <map>

#include "states/state.hpp"

#include "entity/object.hpp"
#include "entity/player.hpp"
#include "graphics2D/image_map.hpp"
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

    TileMap m_tilemap;
    ImageMap m_imageMap;

    std::vector< StaticEntity2D > m_collisionMap;
    Entity2D m_greenEntity;

    Player m_player;

  public:
    EditorState();

    void update_extra( float deltaTime ) override;

  private:
    void update_toolbar();
    void update_debug_window();
    void update_collision_window();
    void update_overlay();
};
