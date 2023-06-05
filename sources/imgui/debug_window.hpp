#pragma once

#include <string>  // for string

#include <fmt/format.h>

#include "imgui/sub_window.hpp"  // for SubWindowZ
// implementation
#include "imgui/imgui.hpp"  // for ImGui::Begin, ImGui::End, ImGui::Text

class DebugWindow : public SubWindow
{
  public:
    DebugWindow( std::string const & name );
    virtual ~DebugWindow() = default;

    // Must not be called inside update_gui(). Just use TextFmt instead
    template< typename... Args >
    void add_debug_text ( fmt::format_string< Args... > fmt, Args &&... args );
};

template< typename... Args >
void DebugWindow::add_debug_text( fmt::format_string< Args... > fmt,
                                  Args &&... args )
{
    if ( ! this->is_enabled() )
    {
        return;
    }

    if ( ImGui::BeginWindow( *this ) )
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
