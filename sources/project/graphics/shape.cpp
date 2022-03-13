#include "shape.hpp"

#include "tools/tools.hpp"

#include <iostream>

Shape::~Shape()
{
    // We delete all the buffers and the objects generated
    glDeleteVertexArrays( 1, &this->m_vertexArrayObject );
    glDeleteBuffers( 1, &this->m_vertexBufferObject );

    if ( this->is_element_buffer_set() )
    {
        glDeleteBuffers( 1, &this->m_elementBufferObject );
    }
}

void Shape::create( std::vector<float> const & vertices,
                    std::vector<unsigned int> const & indices,
                    unsigned int const & numberOfDataPerAttribute )
{
    this->m_numberOfDataPerAttribute = numberOfDataPerAttribute;

    this->m_shader.loadFromFile( tools::get_path::shaders() + "/shader.vert"s,
                                 tools::get_path::shaders() + "/shader.frag"s );

    bool textureLoad { true };
    // TYPO vérifié que cette operateur marche bien
    textureLoad &= this->m_texture.loadFromFile( tools::get_path::resources()
                                                 + "/wall.jpg"s );
    textureLoad &= this->m_texture.generateMipmap();
    if ( ! textureLoad )
    {
        throw std::runtime_error { "Cannot load texture"s };
    }

    // Bind the texture
    sf::Texture::bind( &this->m_texture );

    this->set_vertices( vertices );
    this->set_indices( indices );

    this->objects_generation();
    this->bind_objects();
    this->vertex_shader_attribution();
    this->unbind();
}

void Shape::update( gl::SpaceMatrix const & space )
{
    this->m_space = space;
}

void Shape::draw() const
{
    sf::Texture::bind( &this->m_texture );
    sf::Shader::bind( &this->m_shader );

    glBindVertexArray( this->m_vertexArrayObject );

    this->transform();

    int const primitiveType { GL_TRIANGLES };
    if ( this->is_element_buffer_set() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_elementBufferObject );
        int const dataType { GL_UNSIGNED_INT };
        glDrawElements( primitiveType,
                        static_cast<int>( this->m_vertices.size() ),
                        dataType,
                        0 );
    }
    else
    {
        int const verticesBeginPosition { 0 };
        int const vectorSize { static_cast<int>(
            this->m_vertices.size() / this->m_numberOfDataPerAttribute ) };
        glDrawArrays( primitiveType, verticesBeginPosition, vectorSize );
    }

    sf::Shader::bind( NULL );
    sf::Texture::bind( NULL );
    // Unbind VAO
    glBindVertexArray( 0 );
}

bool Shape::is_element_buffer_set() const
{
    return ! this->m_indices.empty();
}

void Shape::set_vertices( std::vector<float> const & vertices )
{
    this->m_vertices = vertices;
}
void Shape::set_indices( std::vector<unsigned int> const & indices )
{
    this->m_indices = indices;
}

void Shape::objects_generation()
{
    glGenVertexArrays( 1, &this->m_vertexArrayObject );
    glGenBuffers( 1, &this->m_vertexBufferObject );
    if ( this->is_element_buffer_set() )
    {
        glGenBuffers( 1, &this->m_elementBufferObject );
    }
}

void Shape::bind_objects()
{
    // Binding
    glBindVertexArray( this->m_vertexArrayObject );

    int const bufferTarget { GL_ARRAY_BUFFER };
    int const dataExpectedUsage { GL_STATIC_DRAW };

    // VBO
    glBindBuffer( bufferTarget, this->m_vertexBufferObject );
    glBufferData( bufferTarget,
                  static_cast<int>( too::size_of( this->m_vertices ) ),
                  too::to_c_style_array( this->m_vertices ),
                  dataExpectedUsage );

    if ( this->is_element_buffer_set() )
    {
        int const elementTarget { GL_ELEMENT_ARRAY_BUFFER };
        // EBO
        glBindBuffer( elementTarget, this->m_elementBufferObject );
        glBufferData( elementTarget,
                      static_cast<int>( too::size_of( this->m_indices ) ),
                      too::to_c_style_array( this->m_indices ),
                      dataExpectedUsage );
    }
}

void Shape::vertex_shader_attribution()
{
    int const valueType { GL_FLOAT };
    int const hasDataToBeNormalised { GL_FALSE };
    int const dataPerAttributeSize { static_cast<int>(
        this->m_numberOfDataPerAttribute * sizeof( float ) ) };

    {
        unsigned int const positionLocation { 0u };
        unsigned int const positionVectorSize { 3u };
        void * positionOffsetStart { reinterpret_cast<void *>(
            static_cast<intptr_t>( 0 * sizeof( float ) ) ) };

        glVertexAttribPointer( positionLocation,
                               positionVectorSize,
                               valueType,
                               hasDataToBeNormalised,
                               dataPerAttributeSize,
                               positionOffsetStart );
        glEnableVertexAttribArray( positionLocation );
    }

    // {
    //     unsigned int const colorLocation { 1u };
    //     unsigned int const colorVectorSize { 3u };
    //     void * colorOffsetStart { reinterpret_cast<void *>(
    //         static_cast<intptr_t>( 3 * sizeof( float ) ) ) };

    //     glVertexAttribPointer( colorLocation,
    //                            colorVectorSize,
    //                            valueType,
    //                            hasDataToBeNormalised,
    //                            dataPerAttributeSize,
    //                            colorOffsetStart );
    //     glEnableVertexAttribArray( colorLocation );
    // }

    {
        unsigned int const textureCoordinateLocation { 1u };
        unsigned int const textureCoordinateVectorSize { 2u };
        void * textureCoordinateOffsetStart { reinterpret_cast<void *>(
            static_cast<intptr_t>( 3 * sizeof( float ) ) ) };

        glVertexAttribPointer( textureCoordinateLocation,
                               textureCoordinateVectorSize,
                               valueType,
                               hasDataToBeNormalised,
                               dataPerAttributeSize,
                               textureCoordinateOffsetStart );
        glEnableVertexAttribArray( textureCoordinateLocation );
    }
}

void Shape::unbind()
{
    // Unbind
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // je ne sais pas ce que ça fait
    // Unbind VAO
    glBindVertexArray( 0 );
    // Unbind EBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

// TYPO mettre ça autre part
static int get_shader_uniform_location( sf::Shader const & shader,
                                        std::string const & uniformName );

void Shape::transform() const
{
    static float count { 0.5f };

    int const numberOfMatrix { 1 };
    int const transposeMatrix { GL_FALSE };

    int modelLoc = get_shader_uniform_location( this->m_shader, "model"s );
    int viewLoc = get_shader_uniform_location( this->m_shader, "view"s );
    int projectionLoc =
        get_shader_uniform_location( this->m_shader, "projection"s );

    // TYPO utilisé cette fonctions
    // this->m_shader.setUniform( "model"s,
    //                            sf::Glsl::Mat4( this->m_space.model ) );
    glUniformMatrix4fv( modelLoc,
                        numberOfMatrix,
                        transposeMatrix,
                        glm::value_ptr( this->m_space.model ) );
    glUniformMatrix4fv( viewLoc,
                        numberOfMatrix,
                        transposeMatrix,
                        glm::value_ptr( this->m_space.view ) );
    glUniformMatrix4fv( projectionLoc,
                        numberOfMatrix,
                        transposeMatrix,
                        glm::value_ptr( this->m_space.projection ) );

    count += 0.05f;
}

static int get_shader_uniform_location( sf::Shader const & shader,
                                        std::string const & uniformName )
{
    // TYPO check if the returned value is ok
    return glGetUniformLocation( shader.getNativeHandle(),
                                 uniformName.c_str() );
}