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
    void add_text ( fmt::format_string< Args... > fmt, Args &&... args );

    void show ();
    void hide ();

    // This method call show_when_enabled() and must be called 1 time per frame
    // in the update loop
    void         update ();
    // Method that contain elements to show when the window is enabled
    virtual void show_when_enabled ();
};

// implementation
#include "libraries/imgui.hpp"  // for ImGui::Begin, ImGui::End, ImGui::Text

template< typename... Args >
void DebugWindow::add_text( fmt::format_string< Args... > fmt, Args &&... args )
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
