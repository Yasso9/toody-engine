#pragma once

#include "component/component.hpp"
#include "graphics2D/collision/collision_list.hpp"

class CollisionHandler : public Component
{
    CollisionList & m_collisionList;

  public:
    CollisionHandler( CollisionList & collisionList );
    virtual ~CollisionHandler() = default;

  private:
    void update ( float deltaTime ) override;
};