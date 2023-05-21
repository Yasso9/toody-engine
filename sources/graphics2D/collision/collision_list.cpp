#include "collision_list.hpp"

CollisionList::CollisionList() : m_collisionArray {} {}

CollisionList::CollisionList(
    std::vector< StaticEntity2D > const & entityArray )
  : m_collisionArray { entityArray }
{
    for ( StaticEntity2D & entity : m_collisionArray )
    {
        this->add_child( entity );
    }
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
