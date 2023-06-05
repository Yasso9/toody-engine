#pragma once

#include <string>

#include "imgui/imgui.hpp"  // for ImGuiWindowFlags

class SubWindow
{
    std::string      m_windowName;
    bool             m_show;
    ImGuiWindowFlags m_flags;

  public:
    SubWindow( std::string const & name );
    virtual ~SubWindow() = default;

    void   show ();
    void   hide ();
    void   set_enabled ( bool enabled );
    bool   is_enabled () const;
    bool & is_enabled ();
    void   toggle_enabled ();

    void set_window_flags ( ImGuiWindowFlags flags );

    // This method call update_gui() and must be called 1 time per
    // frame in the update loop
    void update_all ();

  protected:
    // Method that contain elements to show when the window is enabled. Should
    // be overrided
    virtual void update_gui ();

    friend bool ImGui::BeginWindow( SubWindow & subWindow );
};
