#pragma once

#include <map>

#include "SFML/Window/Mouse.hpp"
#include "graphics2D/component.hpp"  // for Component2D
#include "graphics2D/view.hpp"       // for View

class Render;

namespace sf
{
    class Event;
}  // namespace sf

class State : public Component2D
{
  public:
    /**
     * @brief List of all States that the game can have
     *        (equal to the number of child to this class)
     */
    enum class E_List
    {
        MainMenu = 0,
        Editor,
        Graphics,
        Test,
        Quit,
        EnumLast,
    };

  private:
    static std::map< std::string, E_List > const m_enumStateListMaps;

  public:
    static E_List                     get_enum_state ( std::string string );
    static std::vector< std::string > get_state_list ();

  private:
    /** @brief value corresponding of the state that the game should run */
    State::E_List                    m_stateName;
    /// @brief List of buttons that have just been pressed
    std::vector< sf::Mouse::Button > m_mouseButtonsPressed;
    /// @brief List of buttons that have just been released
    std::vector< sf::Mouse::Button > m_mouseButtonsReleased;

  protected:
    State( State::E_List const & stateName );

  public:
    /**
     * @brief Know the next state to render after the input update.
     * @returns State::E_List value of the next state to print
     */
    State::E_List get_state_to_print () const;

    void clear_buttons ();
    /// @brief Check if a button has been pressed in the current iteration of
    /// the loop
    /// @param mouseButton Code of the button that have been pressed
    /// @return true if the button has been pressed in the current iteration of
    /// the loop, false otherwise
    bool is_pressed ( sf::Mouse::Button mouseButton ) const;
    /// @brief Check if a button has been released in the current iteration of
    /// the loop
    /// @param mouseButton Code of the button that have been released
    /// @return true if the button has been released in the current iteration of
    /// the loop, false otherwise
    bool is_released ( sf::Mouse::Button mouseButton ) const;

    /// @brief Update any change that can happen by an event. Must be called
    /// in the pollevent function
    void update_inputs ( sf::Event const & event );

  protected:
    void set_new_state ( State::E_List state );

  private:
    virtual void keyboard_pressed ( sf::Event event );
    virtual void keyboard_released ( sf::Event event );

    virtual void mouse_pressed ( sf::Event event );
    virtual void mouse_released ( sf::Event event );
    virtual void mouse_moved ( sf::Event event );

    virtual void mouse_scroll ( float const & deltaScroll );
};