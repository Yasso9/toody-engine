#pragma once

#include <map>     // for map
#include <string>  // for string

#include "graphics2D/collision/collision_list.hpp"  // for CollisionList
#include "graphics2D/entity/character_entity.hpp"
#include "graphics2D/entity/controllable_entity.hpp"  // for MoveableEntity2D
#include "graphics2D/entity/player.hpp"               // for Player
#include "graphics2D/events/dialogue.hpp"
#include "graphics2D/image_map.hpp"  // for ImageMap
#include "graphics2D/tile/map.hpp"   // for TileMap
#include "states/state.hpp"          // for State

class EditorState final : public State
{
    /// @brief 2D camera of the state
    View m_view;

    std::map< std::string, bool > m_showWindow;

    tile::Map m_tilemap;
    ImageMap  m_imageMap;

    CollisionList   m_collisionList;
    // ControllableEntity2D m_greenEntity;
    // Player               m_player;
    CharacterEntity m_character;

    Dialogue m_dialogue;

  public:
    EditorState( GameContext & gameContext );

    void update ( UpdateContext context ) override;

  private:
    void reset_view ( math::Vector2F const & windowSize );
};
