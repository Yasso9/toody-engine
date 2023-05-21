#include "debug_window.hpp"

#include "libraries/imgui.hpp"  // for ImGui::Begin, ImGui::End

DebugWindow::DebugWindow( std::string const & windowName )
  : m_windowName { windowName }, m_show { false }
{}

void DebugWindow::show()
{
    m_show = true;
}

void DebugWindow::hide()
{
    m_show = false;
}

void DebugWindow::update()
{
    if ( ! m_show )
    {
        return;
    }

    if ( ImGui::Begin( m_windowName.c_str(), &m_show ) )
    {
        this->show_when_enabled();
    }
    ImGui::End();
}

void DebugWindow::show_when_enabled() {};
