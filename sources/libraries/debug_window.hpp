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

    void enable_debug ();
    void disable_debug ();

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

// template< typename... Args >
// void DebugWindow::add_log_text( fmt::format_string< Args... > fmt,
//                                 Args &&... args )
// {
//     if ( ! m_show )
//     {
//         return;
//     }

// int nbColumns = 2;
// if ( ImGui::BeginTable( m_windowName + "_separation", nbColumns,
//                         ImGuiTableFlags_None ) )
// {
//     const char * columnNames[nbColumns] = { "Debug", "Logs" };

// for ( int column = 0; column < nbColumns; column++ )
// {
//     ImGui::TableNextColumn();
//     ImGui::PushID( column );
//     // ImGui::AlignTextToFramePadding();  // FIXME-TABLE: Workaround for
//     // wrong text baseline propagation across columns
//     ImGui::Text( "'%s'", columnNames[column] );
//     ImGui::Text( "Input flags:" );
//     EditTableColumnsFlags( &column_flags[column] );
//     ImGui::Spacing();
//     ImGui::Text( "Output flags:" );
//     ImGui::BeginDisabled();
//     ShowTableColumnsStatusFlags( column_flags_out[column] );
//     ImGui::EndDisabled();
//     ImGui::PopID();
// }
// ImGui::EndTable();
// }
// }
