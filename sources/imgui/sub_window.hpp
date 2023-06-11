#pragma once

#include <string>

#include "application/components/component.hpp"  // for Component
#include "imgui/imgui.hpp"                       // for ImGuiWindowFlags

class SubWindow : public Component
{
    std::string      m_windowName;
    bool             m_show;
    ImGuiWindowFlags m_flags;

  public:
    SubWindow( std::string const & name );
    virtual ~SubWindow() = default;

  private:
    void update ( UpdateContext & context ) override final;

  public:
    void   show_window ();
    void   hide_window ();
    void   set_window_enabled ( bool enabled );
    bool   is_window_enabled () const;
    bool & is_window_enabled ();
    void   toggle_window_enabled ();

    void set_window_flags ( ImGuiWindowFlags flags );

  protected:
    // Method that contain elements to show when the window is enabled. Should
    // be overrided
    virtual void update_window ( UpdateContext & context );

    friend bool ImGui::BeginWindow( SubWindow & subWindow );
};
