#include "openGL.hpp"

#include "tools/sfml.hpp"

namespace gl
{
    void initialize()
    {
        if ( ! gladLoadGLLoader(
                 reinterpret_cast<GLADloadproc>( sf::Context::getFunction ) ) )
        {
            throw std::runtime_error { "Failed to initialize GLAD" };
        }

        // configure global opengl state
        // -----------------------------
        glEnable( GL_DEPTH_TEST );
    }

    void clear_window()
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void set_wireframe() { glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); }

    void remove_wireframe() { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }
} // namespace gl

std::ostream & operator<<( std::ostream & stream, glm::vec3 const & vector3 )
{
    return stream << "( " << vector3.x << ", " << vector3.y << ", " << vector3.z
                  << " )";
}
