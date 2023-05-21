#pragma once

#include <string>  // for string

#include <fmt/format.h>

class DebugWindow
{
    std::string m_windowName;
    bool        m_show;

  public:
    DebugWindow( std::string const & windowName );
    virtual ~DebugWindow() = default;

    template< typename... Args >
    void add_debug_text ( fmt::format_string< Args... > fmt, Args &&... args );

    void show_debug ();
    void hide_debug ();

    // This method call debug_window_content() and must be called 1 time per
    // frame in the update loop
    void         update_debug ();
    // Method that contain elements to show when the window is enabled
    virtual void debug_window_content ();
};

// implementation
#include "libraries/imgui.hpp"  // for ImGui::Begin, ImGui::End, ImGui::Text

template< typename... Args >
void DebugWindow::add_debug_text( fmt::format_string< Args... > fmt,
                                  Args &&... args )
{
    if ( ! m_show )
    {
        return;
    }

    if ( ImGui::Begin( m_windowName.c_str(), &m_show ) )
    {
        ImGui::TextFmt( fmt, std::forward< Args >( args )... );
    }
    ImGui::End();
}
