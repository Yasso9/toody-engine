#pragma once

#include <map>     // for map
#include <string>  // for string

#include "graphics2D/collision/collision_list.hpp"  // for CollisionList
#include "graphics2D/entity/character_entity.hpp"
#include "graphics2D/entity/controllable_entity.hpp"  // for MoveableEntity2D
#include "graphics2D/entity/player.hpp"               // for Player
#include "graphics2D/events/dialogue.hpp"
#include "graphics2D/image_map.hpp"      // for ImageMap
#include "graphics2D/tile/map.hpp"       // for TileMap
#include "graphics2D/view_settings.hpp"  // for ViewSettings
#include "states/state.hpp"              // for State

class EditorState : public State
{
    struct ShowWindow
    {
        bool demo;
        bool overlay;
        bool view;
    };

    // 2D camera
    View         m_view;
    ViewSettings m_viewSettings;

    ShowWindow m_showWindow;

    tile::Map m_tilemap;
    ImageMap  m_imageMap;

    CollisionList m_collisionList;
    // ControllableEntity2D m_greenEntity;
    // Player               m_player;
    CharacterEntity m_character;

  public:
    EditorState();
    ~EditorState() = default;

    void update ( UpdateContext & context ) override;

  private:
    void reset_view ( math::Vector2F const & windowSize );

    void update_toolbar ( UpdateContext & context );
    void update_view ( UpdateContext & context );
    void update_overlay ( UpdateContext & context );
};
