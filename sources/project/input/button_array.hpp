#include <project/input/button.hpp>

class ButtonArray final
{
  public:
    ButtonArray() = default;
    ButtonArray( sf::Font const & font,
                 std::vector<std::string> const & buttonsString = {
                     "Button" } );

    /** @brief Set the size of each button of ButtonArray
     * @param size size of each button of ButtonArray
     */
    void set_size( sf::Vector2f const & size );
    /** @brief Set the position of the first button of ButtonArray,
     * i.e where the array start
     * @param position Position of the first button of ButtonArray
     */
    void set_position( sf::Vector2f const & position );

    /** @brief Check if the mouse's click is inside the button.
     * @param position position of the mouse's click
     * @returns key of the button pressed, if none, return -1
     */
    int update_press( sf::Vector2f const & position );

    /** @brief Check if the mouse's click is inside the button.
     * @param position position of the mouse's click
     * @param buttonNumber number that you want to get if the click is outside
     * all the buttons
     * @returns key of the button pressed, if none, return buttonNumer
     */
    int update_press( sf::Vector2f const & position, int const & buttonNumber );

    /** @brief Check if the mouse's actual position is inside the button.
     * @param position Position of the mouse's position
     */
    void update_select( sf::Vector2f const & position );

    void render( sf::RenderWindow & target );

  private:
    std::map<unsigned int, Button> m_buttons {};
    sf::Vector2f m_size {};
    sf::Vector2f m_position {};

    void reset_press();
    void reset_selection();
};