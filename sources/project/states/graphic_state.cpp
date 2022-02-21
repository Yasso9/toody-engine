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
    this->update_camera();
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

        { "AngleUp", { sf::Keyboard::Up, false } },
        { "AngleDown", { sf::Keyboard::Down, false } },
        { "AngleLeft", { sf::Keyboard::Left, false } },
        { "AngleRight", { sf::Keyboard::Right, false } },

        { "ZoomIn", { sf::Keyboard::P, false } },
        { "ZoomOut", { sf::Keyboard::M, false } },
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

void GraphicState::update_camera()
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

    spaceMatrix.model = glm::rotate( glm::mat4 { 1.f },
                                     glm::radians( 50.0f ),
                                     glm::vec3 { 0.5f, 1.f, 0.f } );

    this->m_shape.update( spaceMatrix );
}

void GraphicState::handle_current_input()
{
    float const deltaTime { 0.016f };

    if ( this->m_keyboard.at( "Forward" ).second )
    {
        this->m_camera.move( Camera::E_Movement::Forward, deltaTime );
    }
    if ( this->m_keyboard.at( "Backward" ).second )
    {
        this->m_camera.move( Camera::E_Movement::Backward, deltaTime );
    }
    if ( this->m_keyboard.at( "Left" ).second )
    {
        this->m_camera.move( Camera::E_Movement::Left, deltaTime );
    }
    if ( this->m_keyboard.at( "Right" ).second )
    {
        this->m_camera.move( Camera::E_Movement::Right, deltaTime );
    }

    if ( this->m_keyboard.at( "AngleUp" ).second )
    {
        this->m_camera.rotation( { 0.f, 1.f, 0.f }, deltaTime );
    }
    if ( this->m_keyboard.at( "AngleDown" ).second )
    {
        this->m_camera.rotation( { 0.f, -1.f, 0.f }, deltaTime );
    }
    if ( this->m_keyboard.at( "AngleLeft" ).second )
    {
        this->m_camera.rotation( { -1.f, 0.f, 0.f }, deltaTime );
    }
    if ( this->m_keyboard.at( "AngleRight" ).second )
    {
        this->m_camera.rotation( { 1.f, 0.f, 0.f }, deltaTime );
    }

    if ( this->m_keyboard.at( "ZoomIn" ).second )
    {
        this->m_camera.zoom( 1.f, deltaTime );
    }
    if ( this->m_keyboard.at( "ZoomOut" ).second )
    {
        this->m_camera.zoom( -1.f, deltaTime );
    }
}