#include "graphic_state.hpp"

#include <cmath>
#include <iostream>

GraphicState::GraphicState( std::shared_ptr<sf::RenderWindow> window,
                            Ressources const & ressources,
                            Settings const & settings )
  : State( window, ressources, settings, State::E_List::Graphics )
{
    gl::initialize();

    this->init_shape();
}

void GraphicState::update( float const & /* deltaTime */ )
{
    gl::SpaceMatrix spaceMatrix {};

    float const screenRatio { static_cast<float>(
        this->m_window.get()->getSize().x
        / this->m_window.get()->getSize().y ) };
    // pass projection matrix to shader (note that in this case it could change every frame)
    spaceMatrix.projection =
        glm::perspective( glm::radians( this->m_camera.get_zoom() ),
                          screenRatio,
                          0.1f,
                          100.0f );

    // camera/view transformation
    spaceMatrix.view = this->m_camera.get_view_matrix();

    spaceMatrix.model = glm::rotate( spaceMatrix.model,
                                     glm::radians( 50.0f ),
                                     glm::vec3( 0.5f, 1.0f, 0.0f ) );

    // glm::mat4 view { 1.f };
    // view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );

    // float const screenRatio { static_cast<float>( windowSize.x
    //                                               / windowSize.y ) };
    // glm::mat4 projection { 1.f };
    // projection =
    //     glm::perspective( glm::radians( 45.0f ), screenRatio, 0.1f, 100.0f );

    this->m_shape.update( spaceMatrix );
}

void GraphicState::render()
{
    gl::clear_window();

    this->m_shape.draw();
}

T_KeyboardInputMap GraphicState::init_keyboard_action() const
{
    return {
        { "Forward", { sf::Keyboard::Z, false } },
        { "Backward", { sf::Keyboard::S, false } },
        { "Left", { sf::Keyboard::Q, false } },
        { "Right", { sf::Keyboard::D, false } },
    };
}

T_MouseInputMap GraphicState::init_mouse_action() const
{
    return {};
}

void GraphicState::init_shape()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // for a square
    std::vector<float> vertices {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
    };
    int const numberOfDataPerAttribute { 5u };

    std::vector<unsigned int> indices {};
    // std::vector<unsigned int> indices {
    //     // Notons que l’on commence à 0!
    //     0u, 1u, 3u, // premier triangle
    //     1u, 2u, 3u // second triangle
    // };
    this->m_shape.create( vertices, indices, numberOfDataPerAttribute );
}

void GraphicState::handle_current_input()
{
    if ( this->m_keyboard.at( "Forward" ).second )
    {
        this->m_camera.process_keyboard( Camera::E_Movement::Forward, 0.016f );
    }
    if ( this->m_keyboard.at( "Backward" ).second )
    {
        this->m_camera.process_keyboard( Camera::E_Movement::Backward, 0.016f );
    }
    if ( this->m_keyboard.at( "Left" ).second )
    {
        this->m_camera.process_keyboard( Camera::E_Movement::Left, 0.016f );
    }
    if ( this->m_keyboard.at( "Right" ).second )
    {
        this->m_camera.process_keyboard( Camera::E_Movement::Right, 0.016f );
    }
}