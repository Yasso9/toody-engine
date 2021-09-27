#include "input_initialization.hpp"

namespace InputInitialization
{
    namespace // Works like private
    {
        std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>>
            main_menu_keyboard()
        {
            return {
                { "Previous", { sf::Keyboard::A, false } },
                { "Next", { sf::Keyboard::Z, false } },
                { "AcceptPress", { sf::Keyboard::Space, false } },
            };
        }

        std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>
            main_menu_mouse_button()
        {
            return {
                { "AcceptClick", { sf::Mouse::Left, false } },
            };
        }

        std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>>
            editor_keyboard()
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
                { "SpriteSheetDown", std::make_pair( sf::Keyboard::Down, false ) },
                { "SpriteSheetLeft", std::make_pair( sf::Keyboard::Left, false ) },
                { "SpriteSheetRight", std::make_pair( sf::Keyboard::Right, false ) },

                { "Profondeur1", std::make_pair( sf::Keyboard::Num1, false ) },
                { "Profondeur2", std::make_pair( sf::Keyboard::Num2, false ) }
            };
        }

        std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>
            editor_mouse_button()
        {
            return {
                { "Action", { sf::Mouse::Left, false } },
            };
        }

        std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>>
            game_keyboard()
        {
            return {
                { "MainMenu", { sf::Keyboard::Escape, false } },

                { "MoveUp", { sf::Keyboard::Z, false } },
                { "MoveDown", { sf::Keyboard::S, false } },
                { "MoveLeft", { sf::Keyboard::Q, false } },
                { "MoveRight", { sf::Keyboard::R, false } },

                { "Run", { sf::Keyboard::Space, false } },
            };
        }

        std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>
            game_mouse_button()
        {
            return {

            };
        }
    } // namespace

    std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>> keyboard(
        StateName const & stateName )
    {
        switch ( stateName )
        {
        case StateName::MainMenu : return main_menu_keyboard(); break;
        case StateName::Game : return game_keyboard(); break;
        case StateName::Editor : return editor_keyboard(); break;
        default :
            throw std::invalid_argument(
                "StateName invalid for input initialization" );
            break;
        }
    }

    std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>
        mouse_button( StateName const & stateName )
    {
        switch ( stateName )
        {
        case StateName::MainMenu : return main_menu_mouse_button(); break;
        case StateName::Game : return game_mouse_button(); break;
        case StateName::Editor : return editor_mouse_button(); break;
        default :
            throw std::invalid_argument(
                "StateName invalid for input initialization" );
            break;
        }
    }
} // namespace InputInitialization