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

    // We use a constructor that is only used by childs instead
    // This is an abstract class
    State() = delete;
    virtual ~State() = default;

    /**
     * @brief Know the next state to render after the input update.
     * @returns State::E_List value of the next state to print
     */
    State::E_List get_state_to_print() const;

    /// @brief Update all change that can happen by an event.
    void update_inputs( sf::Event const & event );
    /// @brief Update deltaTime and call the update method
    void update_data( float const & deltaTime );

    /// @brief Draw all the things that we have to draw in our window
    virtual void render() = 0;

  protected:
    State( std::shared_ptr<sf::RenderWindow> window,
           Ressources const & ressources, State::E_List const & stateName );

    std::shared_ptr<sf::RenderWindow> m_window;

    Ressources const m_ressources;
    // TYPO faire rentré les settings dans ressources
    Settings const m_settings;

    /** @brief value corresponding of the state that the game should run */
    State::E_List m_stateName;

    float m_deltaTime;

    virtual void keyboard_pressed( sf::Event event );
    virtual void keyboard_released( sf::Event event );

    virtual void mouse_pressed( sf::Event event );
    virtual void mouse_released( sf::Event event );

    virtual void mouse_scroll( float const & deltaScroll );

    /// @brief update general events
    virtual void extra_events();

    /// @brief Update any change that can happen in the current state.
    virtual void update() = 0;
};