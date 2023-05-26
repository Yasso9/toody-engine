#pragma once

#include "graphics2D/entity/static_entity.hpp"

class CollisionList : public Component
{
    std::vector< StaticEntity2D > m_collisionArray;

  public:
    CollisionList();
    CollisionList( std::vector< StaticEntity2D > const & entityArray );

    std::vector< StaticEntity2D > const & get_entities () const;
    void set_customisation ( bool isCustomisable );
};
