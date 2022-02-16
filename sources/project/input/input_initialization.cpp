#include "input_initialization.hpp"

#include "states/state.hpp"
#include "tools/assertion.hpp"

static T_KeyboardInputMap main_menu_keyboard();
static T_MouseInputMap main_menu_mouse_button();
static T_KeyboardInputMap editor_keyboard();
static T_MouseInputMap editor_mouse_button();
static T_KeyboardInputMap game_keyboard();
static T_MouseInputMap game_mouse_button();

namespace InputInitialization
{
    T_KeyboardInputMap keyboard( State::E_List const & stateName )
    {
        switch ( stateName )
        {
        case State::E_List::MainMenu :
            return main_menu_keyboard();
        case State::E_List::Game :
            return game_keyboard();
        case State::E_List::Editor :
            return editor_keyboard();
        case State::E_List::Graphics :
            break;
        default :
            std::string const debugString {
                "State::E_List unsupported for input initialization"s
            };
            ASSERTION( false, debugString );
            break;
        }

        return {};
    }

    T_MouseInputMap mouse_button( State::E_List const & stateName )
    {
        switch ( stateName )
        {
        case State::E_List::MainMenu :
            return main_menu_mouse_button();
        case State::E_List::Game :
            return game_mouse_button();
        case State::E_List::Editor :
            return editor_mouse_button();
        case State::E_List::Graphics :
            break;
        default :
            ASSERTION( false,
                       "State::E_List unsupported for input initialization" );
            break;
        }

        return {};
    }
} // namespace InputInitialization

static T_KeyboardInputMap main_menu_keyboard()
{
    return {
        { "Previous", { sf::Keyboard::A, false } },
        { "Next", { sf::Keyboard::Z, false } },
        { "AcceptPress", { sf::Keyboard::Space, false } },
    };
}

static T_MouseInputMap main_menu_mouse_button()
{
    return {
        { "AcceptClick", { sf::Mouse::Left, false } },
    };
}

static T_KeyboardInputMap editor_keyboard()
{
    return { { "MainMenu", std::make_pair( sf::Keyboard::Escape, false ) },
             { "PrintOrRemoveSpriteSheet",
               std::make_pair( sf::Keyboard::Space, false ) },
             { "Save", std::make_pair( sf::Keyboard::Enter, false ) },

             { "Center", std::make_pair( sf::Keyboard::C, false ) },
             { "Resize", std::make_pair( sf::Keyboard::P, false ) },

             { "TilemapUp", std::make_pair( sf::Keyboard::Z, false ) },
             { "TilemapDown", std::make_pair( sf::Keyboard::S, false ) },
             { "TilemapLeft", std::make_pair( sf::Keyboard::Q, false ) },
             { "TilemapRight", std::make_pair( sf::Keyboard::D, false ) },

             { "SpriteSheetUp", std::make_pair( sf::Keyboard::Up, false ) },
             { "SpriteSheetDown", std::make_pair( sf::Keyboard::Down, false ) },
             { "SpriteSheetLeft", std::make_pair( sf::Keyboard::Left, false ) },
             { "SpriteSheetRight",
               std::make_pair( sf::Keyboard::Right, false ) },

             { "Profondeur1", std::make_pair( sf::Keyboard::Num1, false ) },
             { "Profondeur2", std::make_pair( sf::Keyboard::Num2, false ) } };
}

static T_MouseInputMap editor_mouse_button()
{
    return {
        { "Action", { sf::Mouse::Left, false } },
    };
}

static T_KeyboardInputMap game_keyboard()
{
    return {
        { "MainMenu", { sf::Keyboard::Escape, false } },

        { "MoveUp", { sf::Keyboard::Z, false } },
        { "MoveDown", { sf::Keyboard::S, false } },
        { "MoveLeft", { sf::Keyboard::Q, false } },
        { "MoveRight", { sf::Keyboard::D, false } },

        { "Run", { sf::Keyboard::Space, false } },
    };
}

static T_MouseInputMap game_mouse_button()
{
    return {

    };
}