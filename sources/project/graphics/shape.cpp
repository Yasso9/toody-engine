#include "shape.hpp"

#include <iostream>
#include <numeric>

#include "tools/assertion.hpp"
#include "tools/tools.hpp"

template <typename ArrayType>
static void bind_buffer_object( unsigned int const & bufferObjectID,
                                GLenum const & target,
                                std::vector<ArrayType> const & dataArray );

unsigned int Shape::Data::get_data_per_point_sum() const
{
    return std::accumulate( this->dataPerPoint.begin(),
                            this->dataPerPoint.end(),
                            0u );
}

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

void Shape::create( Data const & data )
{
    this->m_data = data;

    this->load_textures_and_shaders();

    this->objects_generation();
    this->objects_binding();
    this->vertex_shader_attribution();
    this->unbind();
}

void Shape::load_textures_and_shaders()
{
    this->m_shader.loadFromFile( tools::get_path::shaders() + "/shader.vert"s,
                                 tools::get_path::shaders() + "/shader.frag"s );

    bool textureLoad { true };
    textureLoad &= this->m_texture.loadFromFile( tools::get_path::resources()
                                                 + "/wall.jpg"s );
    textureLoad &= this->m_texture.generateMipmap();
    if ( ! textureLoad )
    {
        throw std::runtime_error { "Cannot load texture"s };
    }
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
                        static_cast<int>( this->m_data.vertices.size() ),
                        dataType,
                        0 );
    }
    else
    {
        int const verticesBeginPosition { 0 };
        int const vectorSize { static_cast<int>(
            this->m_data.vertices.size()
            / this->m_data.get_data_per_point_sum() ) };
        glDrawArrays( primitiveType, verticesBeginPosition, vectorSize );
    }

    // Unbind VAO
    glBindVertexArray( 0 );

    sf::Shader::bind( NULL );
    sf::Texture::bind( NULL );
}

bool Shape::is_element_buffer_set() const
{
    return ! this->m_data.indices.empty();
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

void Shape::objects_binding()
{
    glBindVertexArray( this->m_vertexArrayObject );

    GLenum const bufferTarget { GL_ARRAY_BUFFER };
    bind_buffer_object( this->m_vertexBufferObject,
                        bufferTarget,
                        this->m_data.vertices );

    if ( this->is_element_buffer_set() )
    {
        GLenum const elementTarget { GL_ELEMENT_ARRAY_BUFFER };
        bind_buffer_object( this->m_elementBufferObject,
                            elementTarget,
                            this->m_data.indices );
    }
}

void Shape::vertex_shader_attribution()
{
    int const valueType { GL_FLOAT };
    int const hasDataToBeNormalised { GL_FALSE };
    int const dataAttributeRealSize { static_cast<int>(
        this->m_data.get_data_per_point_sum() * sizeof( float ) ) };

    unsigned int vectorSizeCounter { 0u };

    for ( unsigned int location {}; location < this->m_data.dataPerPoint.size();
          ++location )
    {
        void * offsetStart { reinterpret_cast<void *>(
            static_cast<intptr_t>( vectorSizeCounter * sizeof( float ) ) ) };

        glVertexAttribPointer(
            location,
            static_cast<int>( this->m_data.dataPerPoint[location] ),
            valueType,
            hasDataToBeNormalised,
            dataAttributeRealSize,
            offsetStart );
        glEnableVertexAttribArray( location );

        vectorSizeCounter += this->m_data.dataPerPoint[location];
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

template <typename ArrayType>
static void bind_buffer_object( unsigned int const & bufferObjectID,
                                GLenum const & target,
                                std::vector<ArrayType> const & dataArray )
{
    ASSERTION( target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER,
               "Invalid target argument passed" );

    int const dataExpectedUsage { GL_STATIC_DRAW };

    glBindBuffer( target, bufferObjectID );
    glBufferData( target,
                  static_cast<int>( too::size_of( dataArray ) ),
                  too::to_c_style_array( dataArray ),
                  dataExpectedUsage );
}