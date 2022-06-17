#pragma once

#include <memory>

#include "main/window.hpp"

#include "input/button_array.hpp"

/// @brief abstract class
class State
{
  public:
    /**
     * @brief List of all States that the game can have
     *        (equal to the number of child to this class)
     */
    enum class E_List
    {
        MainMenu = 0,
        Game,
        Editor,
        Graphics,
        Test,
        Quit,
        EnumLast,
    };

    /// @brief We use a constructor that is only used by childs instead
    State()          = delete;
    virtual ~State() = default;

    /**
     * @brief Know the next state to render after the input update.
     * @returns State::E_List value of the next state to print
     */
    State::E_List get_state_to_print() const;

    /// @brief Update all change that can happen by an event.
    void update_inputs( sf::Event const & event );
    /// @brief update general events that are use with sf::Mouse or sf::Button
    ///        (should be outside the pollevent function)
    virtual void extra_events();
    /// @brief Update deltaTime and call the update method
    void update_data( float const & deltaTime );

    /// @brief Update any change that can happen in the current state.
    virtual void update() = 0;
    /// @brief Draw all the things that we have to draw in our window
    virtual void render() const = 0;

  protected:
    State( State::E_List const & stateName );

    /** @brief value corresponding of the state that the game should run */
    State::E_List m_stateName;

    float m_deltaTime;

    virtual void keyboard_pressed( sf::Event event );
    virtual void keyboard_released( sf::Event event );

    virtual void mouse_pressed( sf::Event event );
    virtual void mouse_released( sf::Event event );

    virtual void mouse_scroll( float const & deltaScroll );
};