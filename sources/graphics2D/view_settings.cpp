#include "view_settings.hpp"

#include "libraries/imgui.hpp"

ViewSettings::ViewSettings()
  : m_moveSpeed { 1.f }, m_zoomSpeed { 0.2f }, m_rotateSpeed { 0.2f }
{}

void ViewSettings::sliders()
{
    ImGui::SliderFloat( "View Movement Speed", &m_moveSpeed, 0.f, 50.f,
                        "%.0f" );
    ImGui::SliderFloat( "View Zoom Speed", &m_zoomSpeed, 0.f, 5.f, "%.2f" );
    ImGui::SliderFloat( "View Rotate Speed", &m_rotateSpeed, 0.f, 5.f, "%.2f" );
}

float ViewSettings::get_move_speed() const
{
    return m_moveSpeed;
}

float ViewSettings::get_zoom_speed() const
{
    return m_zoomSpeed;
}

float ViewSettings::get_rotate_speed() const
{
    return m_rotateSpeed;
}

math::Vector2F ViewSettings::get_move_speed_vec() const
{
    return { m_moveSpeed, m_moveSpeed };
}

math::Vector2F ViewSettings::get_zoom_speed_vec() const
{
    return { m_zoomSpeed, m_zoomSpeed };
}

math::Vector2F ViewSettings::get_rotate_speed_vec() const
{
    return { m_rotateSpeed, m_rotateSpeed };
}
