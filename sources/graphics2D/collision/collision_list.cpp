#include "collision_list.hpp"

CollisionList::CollisionList() : m_collisionArray {} {}

CollisionList::CollisionList(
    std::vector< StaticEntity2D > const & entityArray )
  : m_collisionArray { entityArray }
{
    this->add_childs( m_collisionArray );
}

std::vector< StaticEntity2D > const & CollisionList::get_entities() const
{
    return m_collisionArray;
}

void CollisionList::set_customisation( bool isCustomisable )
{
    for ( StaticEntity2D & entity : m_collisionArray )
    {
        entity.set_customisation( isCustomisable );
    }
}