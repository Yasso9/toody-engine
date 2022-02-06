#pragma once

#include <memory>

#include "input/button_array.hpp"
#include "input/mouse_position.hpp"
#include "settings/settings.hpp"
#include "tools/types.hpp"

struct Ressources
{
    Ressources( T_TexturesMap const & v_textures, T_FontsMap const & v_fonts )
      : textures( v_textures ), fonts( v_fonts )
    {}

    T_TexturesMap const textures;
    T_FontsMap const fonts;
};

class State
{
  public:
    // We use a protected constructor instead
    State() = delete;

    /// @brief List of all States that the game can have (equal to the number of child to this class)
    enum class E_List
    {
        MainMenu = 0,
        Game,
        Editor,
        Graphics,
        Quit,
        EnumLast,
    };

    /** @brief Know the next state to render after the input update.
     * @returns State::E_List value of the next state to print */
    State::E_List get_state_to_print() const;

    /// @brief Update all change that can happen by an event.
    void update_input( sf::Event const & event );

    /// @brief Update any change in the state.
    virtual void update() = 0;
    virtual void render() = 0;

    virtual ~State() = default;

  protected:
    State( std::shared_ptr<sf::RenderWindow> window,
           Ressources const & ressources, Settings const & settings,
           State::E_List const & stateName );

    std::shared_ptr<sf::RenderWindow> m_window;

    Ressources const m_ressources;
    Settings const m_settings;

    T_KeyboardInputMap m_keyboard {};
    T_MouseInputMap m_mouseButton {};

    MousePosition m_mousePosition {};

    /** @brief value corresponding of the state that the game should run */
    State::E_List m_stateName {};

    /** @brief Update keyboard and program exit */
    void update_overall_input( sf::Event const & event );

    /** @brief Called when a key in keyboord is pressed
     * Call handle_keyboard_press if a key has not been pressed before.
     * Also set the matched m_keyboard key to true */
    void update_keyboard_pressed( sf::Event const & event );
    /** @brief Called when a key in keyboord is released
     * Set the matched m_keyboard key to false */
    void update_keyboard_released( sf::Event const & event );

    /** @brief Update the actual position of the mouse and
     * the most recent position of press and release
     * of the left mouse button */
    void update_mouse_position( sf::Event const & event );
    /** @brief Called when a button of the mouse is pressed.
     * Call handle_mouse_button_press if the button has not been pressed before.
     * Also set the matched m_mouseButton key to true */
    void update_mouse_button( sf::Event const & event );

    /** @brief To override in the child class
     * Called if a keyboard key is pressed and hasn't been pressed before */
    virtual void handle_keyboard_press( std::string const & inputName );
    /** @brief To override in the child class
     * Called if a mouse button is pressed and hasn't been pressed before */
    virtual void handle_mouse_button_press( std::string const & inputName );
    /** @brief To override in the child class
     * Called if a mouse button is released and has been pressed before */
    virtual void handle_mouse_button_release( std::string const & inputName );

    /** @brief Must be called when the mouse wheel goes up */
    virtual void handle_mouse_wheel_up();
    /** @brief Must be called when the mouse wheel goes down */
    virtual void handle_mouse_wheel_down();

    /** @brief To override in the child class
     * To set for checking the current state of an input */
    virtual void handle_current_input();
    /** @brief To override in the child class
     * To update all additionnal input */
    virtual void update_extra_input( sf::Event const & event );
};