#include "mesh.hpp"

#include <iostream>

Mesh::Mesh( std::vector< S_Vertex > vertices,
            std::vector< unsigned int > indices,
            std::vector< S_Texture > textures )
{
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
}

// render the mesh
void Mesh::draw( sf::Shader const & shader ) const
{
    // bind appropriate textures
    unsigned int diffuseNr { 1u };
    unsigned int specularNr { 1u };
    unsigned int normalNr { 1u };
    unsigned int heightNr { 1u };

    for ( unsigned int i_textureUnit { 0u }; i_textureUnit < textures.size();
          ++i_textureUnit )
    {
        // active proper texture unit before binding
        glActiveTexture( GL_TEXTURE0 + i_textureUnit );
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name { textures[i_textureUnit].type };
        if ( name == "texture_diffuse" )
        {
            number = std::to_string( diffuseNr++ );
        }
        else if ( name == "texture_specular" )
        {
            // transfer unsigned int to string
            number = std::to_string( specularNr++ );
        }
        else if ( name == "texture_normal" )
        {
            // transfer unsigned int to string
            number = std::to_string( normalNr++ );
        }
        else if ( name == "texture_height" )
        {
            // transfer unsigned int to string
            number = std::to_string( heightNr++ );
        }

        // now set the sampler to the correct texture unit
        glUniform1i( glGetUniformLocation( shader.getNativeHandle(),
                                           ( name + number ).c_str() ),
                     static_cast< int >( i_textureUnit ) );
        // and finally bind the texture
        glBindTexture( GL_TEXTURE_2D, textures[i_textureUnit].id );
        // sf::Texture::bind( &textures[i_textureUnit].texture );
    }

    // draw mesh
    glBindVertexArray( VAO );
    Window::get_instance().gl_draw_elements( GL_TRIANGLES,
                                             GL_UNSIGNED_INT,
                                             indices.size() );
    glBindVertexArray( 0 );

    // always good practice to set everything back to defaults once configured.
    glActiveTexture( GL_TEXTURE0 );
}

void Mesh::setupMesh()
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
    glBufferData( GL_ARRAY_BUFFER,
                  static_cast< long >( vertices.size() * sizeof( S_Vertex ) ),
                  &vertices[0],
                  GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast< long >( indices.size() * sizeof( unsigned int ) ),
        &indices[0],
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