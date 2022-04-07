#include "openGL.hpp"

#include "graphics/sfml.hpp"
#include "main/window.hpp"

namespace gl
{
    void initialize()
    {
        // Load glad so we can use openGL function
        if ( ! gladLoadGLLoader( reinterpret_cast< GLADloadproc >(
                 sf::Context::getFunction ) ) )
        {
            throw std::runtime_error { "Failed to initialize GLAD" };
        }

        // enable openGL Z buffer
        glEnable( GL_DEPTH_TEST );

        // Je ne sais pas ce que c'est ?
        glDepthMask( GL_TRUE );
        glClearDepth( 1.f );
        glDisable( GL_LIGHTING );

        // Je ne sais pas ce que c'est ?
        glViewport( 0,
                    0,
                    Window::get_instance().get_size_i().x,
                    Window::get_instance().get_size_i().y );
    }

    void clear_window()
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void set_wireframe() { glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); }

    void remove_wireframe() { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }

    void check_error()
    {
        if ( glGetError() != GL_NO_ERROR )
        {
            throw std::runtime_error { "OpenGL Error" };
        }
    }
} // namespace gl

std::ostream & operator<<( std::ostream & stream, glm::vec3 const & vector3 )
{
    return stream << "( " << vector3.x << ", " << vector3.y << ", " << vector3.z
                  << " )";
}
