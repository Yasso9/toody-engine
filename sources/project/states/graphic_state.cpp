#include "graphic_state.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
// sin
#include <cmath>

#include "graphics/graphics.hpp"
#include "tools/resources.hpp"

// TYPO remplacer les std::cerr par des exceptions

std::string read_file( std::string const & fileName )
{
    std::ifstream file { fileName, std::ios::in };
    if ( ! file )
    {
        std::cerr << "File not found" << fileName << std::endl;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// check for shader compile errors
bool check_shader_error( unsigned int const & shaderValue,
                         unsigned int const & type )
{
    int success;
    glGetProgramiv( shaderValue, type, &success );
    if ( ! success )
    {
        char infoLog[512];
        glGetProgramInfoLog( shaderValue, 512, NULL, infoLog );
        std::cerr << "Error - Shader extraction failed - " << infoLog
                  << std::endl;
    }
    return success;
}

unsigned int compile_shader( unsigned int const & shaderType,
                             std::string const & fileName )
{
    unsigned int shader { glCreateShader( shaderType ) };

    // TYPO : trouver un meilleurs moyen pour convertir un std::string en const char *
    std::string const fileContent { read_file( fileName ) };
    char const * shaderSource { fileContent.c_str() };

    glShaderSource( shader, 1, &shaderSource, NULL );
    glCompileShader( shader );

    check_shader_error( shader, GL_COMPILE_STATUS );

    return shader;
}

unsigned int link_shaders( unsigned int const & vertexShader,
                           unsigned int const & fragmentShader )
{
    unsigned int const shaderProgram { glCreateProgram() };
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );

    glLinkProgram( shaderProgram );
    check_shader_error( shaderProgram, GL_LINK_STATUS );

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return shaderProgram;
}

unsigned int load_shaders( std::string const & vertexShaderPath,
                           std::string const & fragmentShaderPath )
{
    unsigned int const vertexShader { compile_shader( GL_VERTEX_SHADER,
                                                      vertexShaderPath ) };
    unsigned int const fragmentShader { compile_shader( GL_FRAGMENT_SHADER,
                                                        fragmentShaderPath ) };

    return link_shaders( vertexShader, fragmentShader );
}

GraphicState::GraphicState( std::shared_ptr<sf::RenderWindow> window,
                            Ressources const & ressources,
                            Settings const & settings )
  : State( window, ressources, settings, State::E_List::Graphics )
{
    if ( ! gladLoadGLLoader(
             reinterpret_cast<GLADloadproc>( sf::Context::getFunction ) ) )
    {
        std::cerr << "Failed to initialize GLAD"s << std::endl;
    }

    this->m_shaderProgram =
        load_shaders( too::get_shaders_path() + "/shader.vert"s,
                      too::get_shaders_path() + "/shader.frag"s );

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // for a square
    float vertices[] {
        // Position         // Colors
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.5f, 0.5f, 0.5f // top left
    };
    // A square is two triangle. We can only use triangle so we must set them up
    unsigned int indices[] {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3 // second Triangle
    };

    // Generate the vertex arrays and the buffers
    glGenVertexArrays( 1, &this->m_vertexArrayObject );
    glGenBuffers( 1, &this->m_vertexBufferObject );
    glGenBuffers( 1, &this->m_elementBufferObject );

    // We bind the Vertex Array Object first
    glBindVertexArray( this->m_vertexArrayObject );
    // We bind the vertex buffer (the vertices)
    glBindBuffer( GL_ARRAY_BUFFER, this->m_vertexBufferObject );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( vertices ),
                  vertices,
                  GL_STATIC_DRAW );
    // We bind the element buffer (the indices)
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_elementBufferObject );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  sizeof( indices ),
                  indices,
                  GL_STATIC_DRAW );

    // Configure the vertex position attributes(s).
    glVertexAttribPointer( 0,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           3 * sizeof( float ),
                           static_cast<void *>( 0 ) );
    // Disabled by default
    glEnableVertexAttribArray( 0 );

    // Configure the vertex color attributes(s).
    glVertexAttribPointer( 1,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           6 * sizeof( float ),
                           reinterpret_cast<void *>( static_cast<uintptr_t>(
                               3 * sizeof( float ) ) ) );
    glEnableVertexAttribArray( 1 );

    // Disabled by default
    glEnableVertexAttribArray( 1 );

    // note that this is allowed, the call to glVertexAttribPointer
    // registered vertexBufferObject as the vertex attribute's
    // bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // remember: do NOT unbind the EBO while a VAO is active
    // as the bound element buffer object IS stored in the VAO;
    // keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs
    // requires a call to glBindVertexArray anyways so we generally
    // don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray( 0 );

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GraphicState::update( float const & /* deltaTime */ ) {}

void GraphicState::render()
{
    // draw background
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    // draw our first triangle
    glUseProgram( this->m_shaderProgram );

    // update the uniform color
    int vertexColorLocation =
        glGetUniformLocation( this->m_shaderProgram, "ourColor" );
    glUniform4f( vertexColorLocation, 0.0f, 0.2f, 0.8f, 1.0f );

    // seeing as we only have a single VAO there's no need to bind it every time
    // but we'll do so to keep things a bit more organized
    glBindVertexArray( this->m_vertexArrayObject );
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    // glBindVertexArray(0); // no need to unbind it every time
}

GraphicState::~GraphicState()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &this->m_vertexArrayObject );
    glDeleteBuffers( 1, &this->m_vertexBufferObject );
    glDeleteBuffers( 1, &this->m_elementBufferObject );
    glDeleteProgram( this->m_shaderProgram );
}