#pragma once

#include "maths/vector2.hpp"

class ViewSettings
{
    float m_moveSpeed;
    float m_zoomSpeed;
    float m_rotateSpeed;

  public:
    ViewSettings();

    float get_move_speed () const;
    float get_zoom_speed () const;
    float get_rotate_speed () const;

    math::Vector2F get_move_speed_vec () const;
    math::Vector2F get_zoom_speed_vec () const;
    math::Vector2F get_rotate_speed_vec () const;

    void sliders ();
};
