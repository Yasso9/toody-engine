#include "openGL.hpp"

#include <stdexcept>  // for runtime_error
#include <string>     // for allocator, string, operator+

#include <SFML/Graphics/Color.hpp>   // for Color
#include <SFML/Window/Context.hpp>   // for Context
#include <glm/detail/type_vec3.hpp>  // for vec<>::(anonymous)

#include "tools/traces.hpp"

namespace gl
{
    namespace
    {
        class BindVertexArray
        {
          public:
            BindVertexArray( unsigned int vertexArrayObject )
            {
                glBindVertexArray( vertexArrayObject );
                // enable openGL Z buffer
                glEnable( GL_DEPTH_TEST );
            }

            ~BindVertexArray()
            {
                glDisable( GL_DEPTH_TEST );
                glBindVertexArray( 0 );
            }
        };
    }  // namespace

    void draw_elements ( unsigned int vertexArrayObject, GLenum primitiveType,
                         GLenum dataType, std::size_t elementsSize )
    {
        BindVertexArray VAOBinding { vertexArrayObject };

        glDrawElements( primitiveType, static_cast< int >( elementsSize ),
                        dataType, 0 );
    }

    void draw_arrays ( unsigned int vertexArrayObject, GLenum primitiveType,
                       unsigned int arraySize )
    {
        BindVertexArray VAOBinding { vertexArrayObject };

        int const verticesBeginPosition { 0 };
        glDrawArrays( primitiveType, verticesBeginPosition,
                      static_cast< int >( arraySize ) );
    }

    void initialize ( unsigned int const & width, unsigned int const & height )
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

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // Je ne sais pas ce que c'est ?
        glViewport( 0, 0, static_cast< int >( width ),
                    static_cast< int >( height ) );
    }

    void clear_window ( sf::Color const & backgroundColor )
    {
        glClearColor( backgroundColor.r, backgroundColor.g, backgroundColor.b,
                      backgroundColor.a );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void set_wireframe ()
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    void remove_wireframe ()
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    bool check_error ()
    {
        GLenum      errorCode { glGetError() };
        std::string errorMessage { "" };

        if ( errorCode != GL_NO_ERROR )
        {
            errorMessage += "OpenGL - ";

            switch ( errorCode )
            {
            case GL_INVALID_ENUM :
                errorMessage += "Invalid Enum";
                break;
            case GL_INVALID_VALUE :
                errorMessage += "Invalid Value";
                break;
            case GL_INVALID_OPERATION :
                errorMessage += "Invalid Operation";
                break;
            case GL_STACK_OVERFLOW :
                errorMessage += "Stack Overflow";
                break;
            case GL_STACK_UNDERFLOW :
                errorMessage += "Stack Underflow";
                break;
            case GL_OUT_OF_MEMORY :
                errorMessage += "Out Of Memory";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION :
                errorMessage += "Invalid Framebuffer Operation";
                break;
            default :
                errorMessage += "Unkown GL Error";
                break;
            }

            Trace::Error( "{}", errorMessage );
            return false;
        }

        return true;
    }
}  // namespace gl

std::ostream & operator<< ( std::ostream & stream, glm::vec3 const & vector3 )
{
    return stream << "( " << vector3.x << ", " << vector3.y << ", " << vector3.z
                  << " )";
}
