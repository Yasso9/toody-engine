#pragma once

#include <map>     // for map
#include <string>  // for string

#include "game_events/dialogue.hpp"
#include "graphics2D/collision_list.hpp"              // for CollisionList
#include "graphics2D/entity/controllable_entity.hpp"  // for MoveableEntity2D
#include "graphics2D/entity/player.hpp"               // for Player
#include "graphics2D/image_map.hpp"                   // for ImageMap
#include "graphics2D/tile_management/tilemap.hpp"     // for TileMap
#include "states/state.hpp"                           // for State

class EditorState final : public State
{
    /// @brief 2D camera of the state
    View m_view;

    std::map< std::string, bool > m_showWindow;

    TileMap  m_tilemap;
    ImageMap m_imageMap;

    CollisionList        m_collisionList;
    ControllableEntity2D m_greenEntity;
    Player               m_player;

    Dialogue m_dialogue;

  public:
    EditorState();

    void update ( float deltaTime ) override;

  private:
    void reset_view ();
    void update_view ( float deltaTime );

    void update_toolbar ();
    void update_debug_window ();
    void update_collision_window ();
    void update_overlay ( float deltaTime );
};
