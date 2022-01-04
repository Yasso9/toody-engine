#include "input_initialization.hpp"

#include <cassert>

#include <project/states/state.hpp>
#include <project/tools/string.hpp>

namespace InputInitialization
{
    namespace
    {
        KeyboardInputMap main_menu_keyboard()
        {
            return {
                { "Previous", { sf::Keyboard::A, false } },
                { "Next", { sf::Keyboard::Z, false } },
                { "AcceptPress", { sf::Keyboard::Space, false } },
            };
        }

        MouseInputMap main_menu_mouse_button()
        {
            return {
                { "AcceptClick", { sf::Mouse::Left, false } },
            };
        }

        KeyboardInputMap editor_keyboard()
        {
            return {
                { "MainMenu", std::make_pair( sf::Keyboard::Escape, false ) },
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
                { "SpriteSheetDown",
                  std::make_pair( sf::Keyboard::Down, false ) },
                { "SpriteSheetLeft",
                  std::make_pair( sf::Keyboard::Left, false ) },
                { "SpriteSheetRight",
                  std::make_pair( sf::Keyboard::Right, false ) },

                { "Profondeur1", std::make_pair( sf::Keyboard::Num1, false ) },
                { "Profondeur2", std::make_pair( sf::Keyboard::Num2, false ) }
            };
        }

        MouseInputMap editor_mouse_button()
        {
            return {
                { "Action", { sf::Mouse::Left, false } },
            };
        }

        KeyboardInputMap game_keyboard()
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

        MouseInputMap game_mouse_button()
        {
            return {

            };
        }
    } // namespace

    KeyboardInputMap keyboard( StateName const & stateName )
    {
        switch ( stateName )
        {
        case StateName::MainMenu :
            return main_menu_keyboard();
            break;
        case StateName::Game :
            return game_keyboard();
            break;
        case StateName::Editor :
            return editor_keyboard();
            break;
        default :
            std::string const debugString {
                "StateName n "s
                + std::to_string( static_cast<int>( stateName ) )
                + " unsupported for input initialization"s
            };
            assert( false && debugString.c_str() );
            return {};
            break;
        }
    }

    MouseInputMap mouse_button( StateName const & stateName )
    {
        switch ( stateName )
        {
        case StateName::MainMenu :
            return main_menu_mouse_button();
            break;
        case StateName::Game :
            return game_mouse_button();
            break;
        case StateName::Editor :
            return editor_mouse_button();
            break;
        default :
            assert( false && "StateName unsupported for input initialization" );
            return {};
            break;
        }
    }
} // namespace InputInitialization