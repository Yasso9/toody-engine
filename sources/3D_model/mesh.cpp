#include "mesh.hpp"

#include <iostream>

#include "tools/timer.hpp"

Mesh::Mesh( std::vector< S_Vertex > const & vertices,
            std::vector< unsigned int > const & indices,
            std::vector< std::string > const & textures )
  : m_vertices( vertices ), m_indices( indices ), m_textures( textures )
{
    // now that we have all the required data,
    // set the vertex buffers and its attribute pointers.
    this->generate();
}

void Mesh::update( sf::Shader & shader,
                   std::map< std::string, Texture > const & textureLoaded )
{
    // bind appropriate textures
    unsigned int diffuseNr { 1u };
    unsigned int specularNr { 1u };
    unsigned int normalNr { 1u };
    unsigned int heightNr { 1u };

    for ( std::string const & texturePath : this->m_textures )
    {
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        switch ( textureLoaded.at( texturePath ).get_type() )
        {
        case Texture::E_Type::Diffuse :
            number = std::to_string( diffuseNr++ );
            break;
        case Texture::E_Type::Specular :
            number = std::to_string( specularNr++ );
            break;
        case Texture::E_Type::Normal :
            number = std::to_string( normalNr++ );
            break;
        case Texture::E_Type::Height :
            number = std::to_string( heightNr++ );
            break;
        default :
            throw std::runtime_error { "Texture type not supported"s };
            break;
        }

        // Name of the uniform variable in the shader source file
        std::string const uniformVariableName {
            textureLoaded.at( texturePath ).get_type_name() + number
        };

        shader.setUniform( uniformVariableName,
                           textureLoaded.at( texturePath ) );
    }
}

void Mesh::draw() const
{
    // draw mesh
    Window::get_instance().gl_draw_elements( this->VAO,
                                             GL_TRIANGLES,
                                             GL_UNSIGNED_INT,
                                             this->m_indices.size() );
}

void Mesh::generate()
{
    // create buffers/arrays
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

    glBindVertexArray( VAO );
    // load data into vertex buffers
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast< long >( this->m_vertices.size() * sizeof( S_Vertex ) ),
        &m_vertices[0],
        GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast< long >( this->m_indices.size() * sizeof( unsigned int ) ),
        &this->m_indices[0],
        GL_STATIC_DRAW );

    std::vector< unsigned int > dataPerPoint { 3u, 3u, 2u, 3u, 3u, 4u, 4u };
    GLenum const valueType { GL_FLOAT };
    GLenum const hasDataToBeNormalised { GL_FALSE };
    unsigned int const fullSize { sizeof( S_Vertex ) };
    unsigned int vectorSizeCounter { 0u };

    for ( unsigned int location { 0u }; location < 7; ++location )
    {
        void * offsetStart { reinterpret_cast< void * >(
            static_cast< intptr_t >( vectorSizeCounter * sizeof( float ) ) ) };

        glEnableVertexAttribArray( location );
        // TYPO value type for bones change (and function name too)
        glVertexAttribPointer( location,
                               static_cast< int >( dataPerPoint[location] ),
                               valueType,
                               hasDataToBeNormalised,
                               fullSize,
                               offsetStart );

        vectorSizeCounter += dataPerPoint[location];
    }
    glBindVertexArray( 0 );
}