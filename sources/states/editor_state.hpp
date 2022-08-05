#pragma once

#include <map>     // for map
#include <string>  // for string

#include "collision2D/collision_list.hpp"  // for CollisionList
#include "entity/player.hpp"               // for Player
#include "graphics2D/image_map.hpp"        // for ImageMap
#include "graphics2D/moveable_entity.hpp"  // for MoveableEntity2D
#include "graphics2D/tilemap.hpp"          // for TileMap
#include "states/state.hpp"                // for State

class EditorState final : public State
{
    std::map< std::string, bool > m_showWindow;

    TileMap  m_tilemap;
    ImageMap m_imageMap;

    CollisionList    m_collisionList;
    MoveableEntity2D m_greenEntity;

    Player m_player;

  public:
    EditorState();

    void update_before ( float deltaTime ) override;

  private:
    void reset_view ();
    void update_view ( float deltaTime );

    void update_toolbar ();
    void update_debug_window ();
    void update_collision_window ();
    void update_overlay ();
};
