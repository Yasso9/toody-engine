#pragma once

#include "graphics2D/entity/static_entity.hpp"

class CollisionList : public Component
{
    std::vector< StaticEntity2D > m_collisionArray;

  public:
    CollisionList( std::vector< StaticEntity2D > const & entityArray )
      : m_collisionArray { entityArray }
    {
        this->add_childs( m_collisionArray );
    }

    std::vector< StaticEntity2D > const & get_entities () const
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