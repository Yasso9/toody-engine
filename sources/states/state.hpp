#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "graphics2D/component.hpp"
#include "graphics2D/view.hpp"
#include "input/button_array.hpp"
#include "main/window.hpp"
#include "tools/concepts.hpp"

/// @todo inherit from Component
/// @brief abstract class
class State : public Component
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

  protected:
    /** @brief value corresponding of the state that the game should run */
    State::E_List m_stateName;
    View m_view;

    State( State::E_List const & stateName );

  public:
    virtual ~State() = default;

    /**
     * @brief Know the next state to render after the input update.
     * @returns State::E_List value of the next state to print
     */
    State::E_List get_state_to_print() const;

    /// @brief Update any change that can happen by an event. Must be called in the pollevent function
    void update_inputs( sf::Event const & event );

  private:
    void render_before( sf::RenderTarget & target,
                        sf::RenderStates states ) const override;
    void render_after( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;

    virtual void keyboard_pressed( sf::Event event );
    virtual void keyboard_released( sf::Event event );

    virtual void mouse_pressed( sf::Event event );
    virtual void mouse_released( sf::Event event );
    virtual void mouse_moved( sf::Event event );

    virtual void mouse_scroll( float const & deltaScroll );
};