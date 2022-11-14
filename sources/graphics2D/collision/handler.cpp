#include "handler.hpp"

CollisionHandler::CollisionHandler( CollisionList & collisionList )
  : m_collisionList { collisionList }
{}

void CollisionHandler::update( float /* deltaTime */ ) {}