#pragma once

#include "graphics2D/static_entity.hpp"

/// @todo maybe create a base class for CollisionList and CustomCollisionLists
class CollisionList : public Component2D
{
    std::vector< StaticEntity2D > m_collisionArray;

  public:
    CollisionList( std::vector< StaticEntity2D > const & entityArray )
      : m_collisionArray { entityArray }
    {
        this->add_childs( m_collisionArray );
    }

    std::vector< StaticEntity2D > const & get_array () const
    {
        return m_collisionArray;
    }

    void set_customisation ( bool isCustomisable )
    {
        for ( StaticEntity2D & entity : m_collisionArray )
        {
            entity.set_customisation( isCustomisable );
        }
    }
};