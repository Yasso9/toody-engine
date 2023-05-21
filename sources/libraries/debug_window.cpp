#include "debug_window.hpp"

#include "libraries/imgui.hpp"  // for ImGui::Begin, ImGui::End

DebugWindow::DebugWindow( std::string const & windowName )
  : m_windowName { windowName }, m_show { false }
{}

void DebugWindow::show_debug()
{
    m_show = true;
}

void DebugWindow::hide_debug()
{
    m_show = false;
}

void DebugWindow::update_debug()
{
    if ( ! m_show )
    {
        return;
    }

    if ( ImGui::Begin( m_windowName.c_str(), &m_show ) )
    {
        this->debug_window_content();
    }
    ImGui::End();
}

void DebugWindow::debug_window_content() {};
