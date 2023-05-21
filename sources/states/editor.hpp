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

struct ViewSettings
{
    float moveSpeed { 1.f };
    float zoomSpeed { 0.2f };
    float rotateSpeed { 0.2f };

    ViewSettings()          = default;
    virtual ~ViewSettings() = default;

    void edit ()
    {
        ImGui::SliderFloat( "View Movement Speed", &moveSpeed, 0.f, 50.f,
                            "%.0f" );
        ImGui::SliderFloat( "View Zoom Speed", &zoomSpeed, 0.f, 5.f, "%.2f" );
        ImGui::SliderFloat( "View Rotate Speed", &rotateSpeed, 0.f, 5.f,
                            "%.2f" );
    }
};

class EditorState : public State
{
    /// camera
    View         m_view;
    ViewSettings m_viewSettings;

    std::map< std::string, bool > m_showWindow;
    bool                          m_showDemoWindow;
    bool                          m_showViewWindow;

    tile::Map m_tilemap;
    ImageMap  m_imageMap;

    CollisionList   m_collisionList;
    // ControllableEntity2D m_greenEntity;
    // Player               m_player;
    CharacterEntity m_character;

  public:
    EditorState();
    virtual ~EditorState() = default;

    void update ( UpdateContext & context ) override;

  private:
    void reset_view ( math::Vector2F const & windowSize );

    void update_toolbar ( UpdateContext & context );
    void update_view ( UpdateContext & context );
};
