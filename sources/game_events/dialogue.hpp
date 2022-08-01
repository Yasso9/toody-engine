#pragma once

#include <string> // for string

#include <SFML/Graphics/RectangleShape.hpp> // for RectangleShape
#include <SFML/Graphics/Text.hpp>           // for Text
#include <SFML/Window/Cursor.hpp>           // for Cursor

#include "graphics2D/component.hpp" // for Component2D
#include "maths/vector2.hpp"        // for Vector2I

class Render;

class Dialogue : public Component2D
{
  private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    bool m_showCustomisation;

    std::string m_textRemaining;

    /// @todo create a singleton class where we can access all type of cursor (or set all type of cursor)
    sf::Cursor m_regularCursor;
    sf::Cursor m_moveCursor;

  public:
    /// @ number of character that can be print on a single dialogue box
    unsigned int const CHARACTER_LIMIT { 150u };

    Dialogue();
    virtual ~Dialogue() = default;

    /**
     * @brief go to the next dialogue section if available
     * @return true if there is something else to read
     * @return false if there is nothing else to read
     */
    bool next();

    /// @todo create an inherited class DialogueCustmable that can have process_events_customisation and update_customisation and delete this two functions from this class
    void process_mouse_movement_customisation( math::Vector2I mouseMovement );

    void update_before( float /* deltaTime */ ) override;

  private:
    void render_before( Render & render ) const override;

    void set_text( std::string const & text );
};