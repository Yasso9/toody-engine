#pragma once

#include "application/components/component.hpp"  // for Component
#include "tools/enum.hpp"

enum class StateList
{
    MainMenu = 0,
    Editor,
    Graphics,
    Test,
    Quit,
    LAST_ENUM_NAME
};
BOOST_DESCRIBE_ENUM( StateList, MainMenu, Editor, Graphics, Test, Quit,
                     LAST_ENUM_NAME );

class State : public Component
{
  protected:
    State();

  public:
    void update_all ( UpdateContext & context ) override;
    void render_all ( RenderContext & context ) const override;
};
