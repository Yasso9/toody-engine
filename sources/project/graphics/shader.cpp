#include "shader.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "tools/resources.hpp"
#include "tools/sfml.hpp"

// TYPO mettre ça autre part
static std::string read_file( std::string const & fileName )
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

Shaders::Shaders() {}
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
    unsigned int const vertexShader { this->compile( GL_VERTEX_SHADER,
                                                     vertexShaderPath ) };
    unsigned int const fragmentShader { this->compile( GL_FRAGMENT_SHADER,
                                                       fragmentShaderPath ) };

    this->m_id = link( vertexShader, fragmentShader );
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
        std::cerr << "Error - Shader extraction failed - " << infoLog
                  << std::endl;
    }
    return success;
}

unsigned int Shaders::compile( unsigned int const & shaderType,
                               std::string const & fileName )
{
    unsigned int shader { glCreateShader( shaderType ) };

    // TYPO : trouver un meilleurs moyen pour convertir un std::string en const char *
    std::string const fileContent { read_file( fileName ) };
    char const * shaderSource { fileContent.c_str() };

    glShaderSource( shader, 1, &shaderSource, NULL );
    glCompileShader( shader );

    check_error( shader, GL_COMPILE_STATUS );

    return shader;
}

unsigned int Shaders::link( unsigned int const & vertexShader,
                            unsigned int const & fragmentShader )
{
    unsigned int const shaderProgram { glCreateProgram() };
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );

    glLinkProgram( shaderProgram );
    check_error( shaderProgram, GL_LINK_STATUS );

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return shaderProgram;
}