#pragma once

#include "maths/maths.hpp"

class Object2D
{
    math::Vector2F m_position;
    math::Vector2F m_size;

  public:
    Object2D() : m_position( 0.f, 0.f ), m_size( 0.f, 0.f ) {}
};