#include "graphic_state.hpp"

#include <cmath>
#include <iostream>

GraphicState::GraphicState( std::shared_ptr<sf::RenderWindow> window,
                            Ressources const & ressources,
                            Settings const & settings )
  : State( window, ressources, settings, State::E_List::Graphics )
{
    // TYPO mettre ça au début de l'exection du program après l'initialisation du window
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

void GraphicState::init_shape()
{
    // std::vector<float> vertices {
    //     // positions          // colors           // texture coords
    //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    // };
    // unsigned int const numberOfDataPerAttribute { 8u };
    // std::vector<unsigned int> indices {
    //     0, 1, 3, // first triangle
    //     1, 2, 3 // second triangle
    // };

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
    unsigned int const numberOfDataPerAttribute { 5u };

    std::vector<unsigned int> indices {};
    this->m_shape.create( vertices, indices, numberOfDataPerAttribute );
}

void GraphicState::update_camera()
{
    gl::SpaceMatrix spaceMatrix {};

    {
        float const fieldOfView { glm::radians( this->m_camera.get_zoom() ) };
        float const screenRatio { static_cast<float>(
            this->m_window.get()->getSize().x
            / this->m_window.get()->getSize().y ) };
        float const nearDistanceFromCamera { 0.1f };
        float const farDistanceFromCamera { 100.f };

        // space transformation
        spaceMatrix.projection = glm::perspective( fieldOfView,
                                                   screenRatio,
                                                   nearDistanceFromCamera,
                                                   farDistanceFromCamera );
    }

    // camera/view transformation
    spaceMatrix.view = this->m_camera.get_view_matrix();

    {
        glm::mat4 const translation {
            glm::translate( glm::mat4 { 1.f }, glm::vec3 { 0.f, 0.f, 0.f } )
        };
        glm::mat4 const rotation { glm::rotate( glm::mat4 { 1.f },
                                                glm::radians( 0.f ),
                                                glm::vec3 { 1.f, 1.f, 1.f } ) };
        glm::mat4 const scale { glm::scale( glm::mat4 { 1.f },
                                            glm::vec3 { 1.f, 1.f, 1.f } ) };
        // object transformation
        spaceMatrix.model = translation * rotation * scale;
    }

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
        this->m_camera.rotate( { 0.f, 1.f, 0.f }, deltaTime );
    }
    if ( this->m_keyboard.at( "AngleDown" ).second )
    {
        this->m_camera.rotate( { 0.f, -1.f, 0.f }, deltaTime );
    }
    if ( this->m_keyboard.at( "AngleLeft" ).second )
    {
        this->m_camera.rotate( { -1.f, 0.f, 0.f }, deltaTime );
    }
    if ( this->m_keyboard.at( "AngleRight" ).second )
    {
        this->m_camera.rotate( { 1.f, 0.f, 0.f }, deltaTime );
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