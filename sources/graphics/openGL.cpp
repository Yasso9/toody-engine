#include "openGL.hpp"

#include "main/window.hpp"
#include "tools/assertion.hpp"
#include "tools/string.hpp"

namespace gl
{
    void initialize( unsigned int const & width, unsigned int const & height )
    {
        // Load glad so we can use openGL function
        if ( ! gladLoadGLLoader( reinterpret_cast< GLADloadproc >(
                 sf::Context::getFunction ) ) )
        {
            throw std::runtime_error { "Failed to initialize GLAD" };
        }

        // Je ne sais pas ce que c'est mais c'est important ?
        // glDepthMask( GL_TRUE );
        // glClearDepth( 1.f );
        // glDisable( GL_LIGHTING );

        // Je ne sais pas ce que c'est ?
        glViewport( 0,
                    0,
                    static_cast< int >( width ),
                    static_cast< int >( height ) );
    }

    void clear_window( sf::Color const & backgroundColor )
    {
        glClearColor( backgroundColor.r,
                      backgroundColor.g,
                      backgroundColor.b,
                      backgroundColor.a );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void set_wireframe() { glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); }

    void remove_wireframe() { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }

    void check_error()
    {
        GLenum errorCode;
        std::string errorMessage { ""s };
        unsigned int numberOfError { 0u };

        if ( ( errorCode = glGetError() ) != GL_NO_ERROR )
        {
            errorMessage +=
                "OpenGL Error "s + std::to_string( ++numberOfError ) + " : "s;

            switch ( errorCode )
            {
            case GL_INVALID_ENUM :
                errorMessage += "Invalid Enum"s;
                break;
            case GL_INVALID_VALUE :
                errorMessage += "Invalid Value"s;
                break;
            case GL_INVALID_OPERATION :
                errorMessage += "Invalid Operation"s;
                break;
            case GL_STACK_OVERFLOW :
                errorMessage += "Stack Overflow"s;
                break;
            case GL_STACK_UNDERFLOW :
                errorMessage += "Stack Underflow"s;
                break;
            case GL_OUT_OF_MEMORY :
                errorMessage += "Out Of Memory"s;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION :
                errorMessage += "Invalid Framebuffer Operation"s;
                break;
            default :
                errorMessage += "Unkown GL Error"s;
                break;
            }
            errorMessage += " | ";
        }

        ASSERTION( numberOfError == 0u, errorMessage );
    }
} // namespace gl

std::ostream & operator<<( std::ostream & stream, glm::vec3 const & vector3 )
{
    return stream << "( " << vector3.x << ", " << vector3.y << ", " << vector3.z
                  << " )";
}
