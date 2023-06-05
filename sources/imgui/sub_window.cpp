#include "sub_window.hpp"

#include "imgui/imgui.hpp"  // for ImGui::Begin, ImGui::End

SubWindow::SubWindow( std::string const & name )
  : m_windowName { name },
    m_show { false },
    m_flags { ImGui::DEFAULT_WINDOW_FLAGS }
{}

void SubWindow::show()
{
    m_show = true;
}

void SubWindow::hide()
{
    m_show = false;
}

void SubWindow::set_enabled( bool enabled )
{
    m_show = enabled;
}

bool SubWindow::is_enabled() const
{
    return m_show;
}

bool & SubWindow::is_enabled()
{
    return m_show;
}

void SubWindow::toggle_enabled()
{
    m_show = ! m_show;
}

void SubWindow::set_window_flags( ImGuiWindowFlags flags )
{
    m_flags = flags;
}

void SubWindow::update_all()
{
    if ( ! is_enabled() )
    {
        return;
    }

    if ( ImGui::BeginWindow( *this ) )
    {
        this->update_gui();
    }
    ImGui::End();
}

void SubWindow::update_gui() {};
