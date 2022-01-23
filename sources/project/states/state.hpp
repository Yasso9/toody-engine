#pragma once

#include <project/input/button_array.hpp>
#include <project/input/input_initialization.hpp>
#include <project/input/mouse_position.hpp>
#include <project/settings/settings.hpp>

struct Ressources
{
    Ressources( TexturesMap const & v_textures, FontsMap const & v_fonts )
      : textures( v_textures ), fonts( v_fonts )
    {
    }

    TexturesMap const textures;
    FontsMap const fonts;
};

class State
{
  public:
    State() = delete;
    State( const State & ) noexcept = delete;
    State( State && ) noexcept = delete;
    State & operator=( const State & ) = delete;
    State & operator=( State && ) noexcept = delete;

    /** @brief Know the next state to render after the input update.
     * @returns StateName value of the next state to print */
    StateName get_state_to_print() const;

    /// @brief Update all change that can happen by an event.
    void update_input( sf::Event const & event );

    /// @brief Update any change in the state.
    virtual void update() = 0;
    virtual void render( sf::RenderWindow & target ) = 0;

    virtual ~State() noexcept = default;

  protected:
    State( Ressources const & ressources, Settings const & settings,
           StateName const & stateName );

    Ressources const m_ressources;
    Settings const m_settings;

    std::map<std::string const, std::pair<sf::Keyboard::Key const, bool>>
        m_keyboard {};
    std::map<std::string const, std::pair<sf::Mouse::Button const, bool>>
        m_mouseButton {};

    MousePosition m_mousePosition {};

    /** @brief value corresponding of the state that the game should run */
    StateName m_stateName {};

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