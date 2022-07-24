#pragma once

#include <filesystem>
#include <map>
#include <string>

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
    std::map< std::string, bool > m_showWindow;

    TileMap m_tilemap;
    ImageMap m_imageMap;

    std::vector< StaticEntity2D > m_collisionMap;
    Entity2D m_greenEntity;

    Player m_player;

  public:
    EditorState();

    void update_before( float deltaTime ) override;

  private:
    void reset_view();
    void update_view( float deltaTime );

    void update_toolbar();
    void update_debug_window();
    void update_collision_window();
    void update_overlay();
};
