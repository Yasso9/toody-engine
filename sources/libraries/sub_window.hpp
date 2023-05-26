#pragma once

#include <string>

class SubWindow
{
  public:
    std::string windowName;
    bool        showWindow;

    SubWindow( std::string const & name, bool show = false );
    virtual ~SubWindow() = default;

    virtual void update_gui_window () = 0;
};
