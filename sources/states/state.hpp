#pragma once

#include <boost/describe.hpp>  // for BOOST_DESCRIBE_NESTED_ENUM

#include "components/component.hpp"  // for Component

class State : public Component
{
  public:
    enum E_List
    {
        MainMenu = 0,
        Editor,
        Graphics,
        Test,
        Quit,
        EnumLast,
    };

    BOOST_DESCRIBE_NESTED_ENUM( E_List, MainMenu, Editor, Graphics, Test, Quit,
                                EnumLast );
    static E_List                     get_enum_state ( std::string string );
    static std::vector< std::string > get_state_list ();

  protected:
    State();

  public:
    void update_all ( UpdateContext & context ) override;
    void render_all ( RenderContext & context ) const override;
};
