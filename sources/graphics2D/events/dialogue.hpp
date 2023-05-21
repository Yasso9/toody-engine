#pragma once

#include <string>  // for string

#include <SFML/Graphics/RectangleShape.hpp>  // for RectangleShape
#include <SFML/Graphics/Text.hpp>            // for Text
#include <SFML/Window/Cursor.hpp>            // for Cursor

#include "components/component.hpp"  // for Component
#include "maths/vector2.hpp"         // for Vector2I

class View;
class Render;

class Dialogue : public Component
{
    sf::RectangleShape m_shape;
    sf::Text           m_text;
    /// @brief true if we show the dialogue, false otherwise
    bool               m_isEnabled;
    bool               m_showCustomisation;

    std::string m_textRemaining;

    sf::Cursor m_regularCursor;
    sf::Cursor m_moveCursor;

  public:
    /// @brief number of character that can be print on a single dialogue box
    unsigned int const CHARACTER_LIMIT { 150u };

    Dialogue();
    virtual ~Dialogue() = default;

  private:
    void update ( UpdateContext & context ) override;
    void render ( RenderContext & context ) const override;

  public:
    void enable ();
    void disable ();
    void set_enabled ( bool isEnabled );
    bool is_enabled () const;

    /**
     * @brief go to the next dialogue section if available
     * @return true if there is something else to read
     * @return false if there is nothing else to read
     */
    bool next ();

    void add_text ( std::string textToAdd );

    void process_mouse_movement_customisation ( math::Vector2I mouseMovement );

  private:
    void set_current_text ( std::string const & text );
};
