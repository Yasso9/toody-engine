#pragma once

#include <vector>

#include "graphics/openGL.hpp"
#include "graphics/sfml.hpp"
#include "main/window.hpp"
#include "tools/string.hpp"

#define MAX_BONE_INFLUENCE 4

struct S_Vertex
{
    /// @brief position
    glm::vec3 position;
    /// @brief normal
    glm::vec3 normal;
    /// @brief texCoords
    glm::vec2 textureCoordinates;
    /// @brief tangent
    glm::vec3 tangent;
    /// @brief bitangent
    glm::vec3 bitangent;
    /// @brief bone indexes which will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    /// @brief weights from each bone
    float weights[MAX_BONE_INFLUENCE];
};

struct S_Texture
{
    unsigned int id;
    // sf::Texture texture {};
    std::string type;
    std::string path;
};

class Mesh
{
  public:
    // mesh Data
    std::vector< S_Vertex > vertices;
    std::vector< unsigned int > indices;
    std::vector< S_Texture > textures;
    unsigned int VAO;

    // constructor
    Mesh( std::vector< S_Vertex > vertices, std::vector< unsigned int > indices,
          std::vector< S_Texture > textures )
    {
        this->vertices = vertices;
        this->indices  = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        this->setupMesh();
    }

    // render the mesh
    void draw( sf::Shader const & shader ) const
    {
        // bind appropriate textures
        unsigned int diffuseNr { 1u };
        unsigned int specularNr { 1u };
        unsigned int normalNr { 1u };
        unsigned int heightNr { 1u };

        for ( unsigned int i_textureUnit { 0u };
              i_textureUnit < textures.size();
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
        sf::Texture::bind( NULL );
    }

  private:
    // render data
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
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
            static_cast< long >( vertices.size() * sizeof( S_Vertex ) ),
            &vertices[0],
            GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast< long >( indices.size() * sizeof( unsigned int ) ),
            &indices[0],
            GL_STATIC_DRAW );

        // std::cout << "offset position: " << offsetof( S_Vertex, position )
        //           << std::endl;
        // std::cout << "offset normal: " << offsetof( S_Vertex, normal )
        //           << std::endl;
        // std::cout << "offset textureCoordinates: "
        //           << offsetof( S_Vertex, textureCoordinates ) << std::endl;
        // std::cout << "offset tangent: " << offsetof( S_Vertex, tangent )
        //           << std::endl;
        // std::cout << "offset weighs: " << offsetof( S_Vertex, weights )
        //           << std::endl;

        // for ( unsigned int location { 0u }; location < 7; ++location )
        // {
        //     void * offsetStart { reinterpret_cast< void * >(
        //         static_cast< intptr_t >( vectorSizeCounter
        //                                  * sizeof( float ) ) ) };

        //     glVertexAttribPointer(
        //         location,
        //         static_cast< int >( this->m_data.dataPerPoint[location] ),
        //         valueType,
        //         hasDataToBeNormalised,
        //         dataAttributeRealSize,
        //         offsetStart );
        //     glEnableVertexAttribArray( location );

        //     vectorSizeCounter += this->m_data.dataPerPoint[location];
        // }
        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0,
                               3,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof( S_Vertex ),
                               reinterpret_cast< void * >( 0 ) );
        // vertex normals
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof( S_Vertex ),
            reinterpret_cast< void * >( offsetof( S_Vertex, normal ) ) );
        // vertex texture coords
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2,
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof( S_Vertex ),
                               reinterpret_cast< void * >(
                                   offsetof( S_Vertex, textureCoordinates ) ) );
        // vertex tangent
        glEnableVertexAttribArray( 3 );
        glVertexAttribPointer(
            3,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof( S_Vertex ),
            reinterpret_cast< void * >( offsetof( S_Vertex, tangent ) ) );
        // vertex bitangent
        glEnableVertexAttribArray( 4 );
        glVertexAttribPointer(
            4,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof( S_Vertex ),
            reinterpret_cast< void * >( offsetof( S_Vertex, bitangent ) ) );
        // ids
        glEnableVertexAttribArray( 5 );
        glVertexAttribIPointer(
            5,
            4,
            GL_INT,
            sizeof( S_Vertex ),
            reinterpret_cast< void * >( offsetof( S_Vertex, boneIDs ) ) );

        // weights
        glEnableVertexAttribArray( 6 );
        glVertexAttribPointer(
            6,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof( S_Vertex ),
            reinterpret_cast< void * >( offsetof( S_Vertex, weights ) ) );
        glBindVertexArray( 0 );
    }
};