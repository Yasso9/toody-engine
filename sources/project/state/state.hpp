#pragma once

#include <project/utility/utility.hpp>
#include <project/input/button_vector.hpp>
#include <project/input/mouse_position.hpp>
#include <project/state/input_initialization.hpp>

class State
{
public:
    /** @brief Know the next state to render after the input update.
     * @returns StateName value of the next state to print
     */
    StateName get_next();

    /// @brief Update any change made by the play action.
    void update_input( sf::Event const & event );

    /// @brief Update any change in the state.
    virtual void update() = 0;
    virtual void render( sf::RenderWindow & target ) = 0;

    virtual ~State() = default;

protected:
    State(
        std::map< TextureKey, sf::Texture > const & textures,
        std::map< FontKey, sf::Font > const & fonts,
        sf::Vector2u const & windowSize,
        StateName const & stateName
    );

    std::map< TextureKey, sf::Texture > const m_textures;
    std::map< FontKey, sf::Font > const m_fonts;
    std::map< std::string const, std::pair< sf::Keyboard::Key const, bool > > m_keyboard;
    std::map< std::string const, std::pair< sf::Mouse::Button const, bool > > m_mouseButton;
    sf::Vector2u const m_windowSize;

    MousePosition m_mousePosition {};
    /// @brief value corresponding of the state that the game should run
    StateName m_stateName {};

    /* Met a jour les evenements liés à une touche du clavier.
    Elle est appelé dans update_input(event).
    A noter que c'est la même fonction pour chaque state donc
    elle n'est pas virtuel, si on veut specifier les bonnes input
    en fonctions des states il faut utiliser handle_press_input */
    void update_keyboard_pressed( sf::Event const & event );
    void update_keyboard_released( sf::Event const & event );
    /* Met a jour les evenements liés à la souris
    Cette fonction est appelé dans update_input(event) */
    void update_mouse_button( sf::Event const & event );
    void update_mouse_position( sf::Event const & event );
    /* Permet de mettre à jour d'autres évènement */
    virtual void update_extra_input( sf::Event const & event );

    /* Effectue les actions spécifique lié au touche pressé
    au moment donné efféctué par l'utilisateur. Ne prend en compte
    que les touches qui ne doivent pas être répété
    Elle est appelé par la fonction update_input */
    virtual void handle_keyboard_press( std::string const & inputName );
    /* Effectue les même actions que handle_keyboard_press
    pour les boutons de la souris */
    virtual void handle_mouse_button_press( std::string const & inputName );
    virtual void handle_mouse_button_release( std::string const & inputName );

    /// @brief function called when the mouse wheel goes up
    virtual void handle_mouse_wheel_up();
    /// @brief function called when the mouse wheel goes down
    virtual void handle_mouse_wheel_down();

    /* Effectue les actions général lié au touche
    efféctué par l'utilisateur.
    Cette fonction est appelé par la fonction update
    de la classe State ou sous-class */
    virtual void handle_global_input();
};