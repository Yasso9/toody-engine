#include "shader.hpp"

#include <cstdlib>    // for NULL
#include <stdexcept>  // for runtime_error

#include <glad/glad.h>  // for glAttachShader, glDeleteShader, GL_COMPIL...

#include "tools/tools.hpp"  // for read_file

Shaders::~Shaders()
{
    glDeleteProgram( this->m_id );
}

unsigned int Shaders::get_id() const
{
    return this->m_id;
}

int Shaders::get_uniform_location( std::string const & variableName ) const
{
    return glGetUniformLocation( this->m_id, variableName.c_str() );
}

void Shaders::load( std::string const & vertexShaderPath,
                    std::string const & fragmentShaderPath )
{
    unsigned int const vertexShader {
        this->compile( GL_VERTEX_SHADER, vertexShaderPath ) };
    unsigned int const fragmentShader {
        this->compile( GL_FRAGMENT_SHADER, fragmentShaderPath ) };

    this->m_id = this->link( vertexShader, fragmentShader );
}

void Shaders::use() const
{
    glUseProgram( this->m_id );
}

// check for shader compile errors
bool Shaders::check_error( unsigned int const & shaderValue,
                           unsigned int const & type )
{
    int success;
    glGetProgramiv( shaderValue, type, &success );
    if ( ! success )
    {
        char infoLog[512];
        glGetProgramInfoLog( shaderValue, 512, NULL, infoLog );
        throw std::runtime_error {
            std::string { "Error - Shader extraction failed - " } + infoLog };
    }
    return success;
}

unsigned int Shaders::compile( unsigned int const & shaderType,
                               std::string const &  fileName )
{
    unsigned int shader { glCreateShader( shaderType ) };

    std::string const fileContent { tools::read_file( fileName ) };
    char const *      shaderSource { fileContent.c_str() };

    glShaderSource( shader, 1, &shaderSource, NULL );
    glCompileShader( shader );

    this->check_error( shader, GL_COMPILE_STATUS );

    return shader;
}

unsigned int Shaders::link( unsigned int const & vertexShader,
                            unsigned int const & fragmentShader )
{
    unsigned int const shaderProgram { glCreateProgram() };
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );

    glLinkProgram( shaderProgram );
    this->check_error( shaderProgram, GL_LINK_STATUS );

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return shaderProgram;
}