#include "shape.hpp"

#include <iostream>
#include <numeric>

#include "main/window.hpp"
#include "tools/assertion.hpp"
#include "tools/tools.hpp"

template < typename ArrayType >
static void bind_buffer_object( unsigned int const & bufferObjectID,
                                GLenum const & target,
                                std::vector< ArrayType > const & dataArray );

unsigned int Shape::Data::get_data_per_point_sum() const
{
    return std::accumulate( this->dataPerPoint.begin(),
                            this->dataPerPoint.end(),
                            0u );
}

Shape::Shape()
{
    this->reset_space_model();
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

void Shape::translate( glm::vec3 const & tranlationVector )
{
    glm::mat4 const translationMatrix { glm::translate( glm::mat4 { 1.f },
                                                        tranlationVector ) };

    this->m_spaceModel *= translationMatrix;
}
void Shape::rotate( glm::vec3 const & rotationVector, float const & angle )
{
    glm::mat4 const rotationMatrix { glm::rotate(
        glm::mat4 { 1.f },
        glm::radians( angle ),
        glm::vec3 { rotationVector } ) };

    this->m_spaceModel *= rotationMatrix;
}
void Shape::scale( glm::vec3 const & scaleVector )
{
    glm::mat4 const scaleMatrix { glm::scale( glm::mat4 { 1.f },
                                              glm::vec3 { scaleVector } ) };
    this->m_spaceModel *= scaleMatrix;
}

void Shape::update( glm::mat4 const & projection, glm::mat4 const & view )
{
    this->m_space.projection = projection;
    this->m_space.view       = view;
    this->m_space.model      = this->m_spaceModel;

    // All the object transformation have been made, so we reset the matrix to identity
    this->reset_space_model();

    this->transform();
}

void Shape::draw() const
{
    sf::Texture::bind( &this->m_texture );
    sf::Shader::bind( &this->m_shader );

    glBindVertexArray( this->m_vertexArrayObject );

    GLenum const primitiveType { GL_TRIANGLES };
    if ( this->is_element_buffer_set() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_elementBufferObject );
        GLenum const dataType { GL_UNSIGNED_INT };
        Window::get_instance().gl_draw_elements( primitiveType,
                                                 dataType,
                                                 this->m_data.vertices.size() );
    }
    else
    {
        unsigned int const vectorSize {
            this->m_data.vertices.size() / this->m_data.get_data_per_point_sum()
        };
        Window::get_instance().gl_draw_arrays( primitiveType, vectorSize );
    }

    // Unbind VAO
    glBindVertexArray( 0 );

    sf::Shader::bind( NULL );
    sf::Texture::bind( NULL );
}

void Shape::load_textures_and_shaders()
{
    this->m_shader.loadFromFile( tools::get_path::shaders( "shader.vert"s ),
                                 tools::get_path::shaders( "shader.frag"s ) );

    bool textureLoad { true };
    textureLoad &= this->m_texture.loadFromFile(
        tools::get_path::resources( "wall.jpg"s ) );
    textureLoad &= this->m_texture.generateMipmap();
    if ( ! textureLoad )
    {
        throw std::runtime_error { "Cannot load texture"s };
    }
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
    int const dataAttributeRealSize { static_cast< int >(
        this->m_data.get_data_per_point_sum() * sizeof( float ) ) };

    unsigned int vectorSizeCounter { 0u };

    for ( unsigned int location { 0u };
          location < this->m_data.dataPerPoint.size();
          ++location )
    {
        void * offsetStart { reinterpret_cast< void * >(
            static_cast< intptr_t >( vectorSizeCounter * sizeof( float ) ) ) };

        glVertexAttribPointer(
            location,
            static_cast< int >( this->m_data.dataPerPoint[location] ),
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

void Shape::transform()
{
    this->m_shader.setUniform(
        "model"s,
        sf::Glsl::Mat4 { glm::value_ptr( this->m_space.model ) } );
    this->m_shader.setUniform(
        "view"s,
        sf::Glsl::Mat4 { glm::value_ptr( this->m_space.view ) } );
    this->m_shader.setUniform(
        "projection"s,
        sf::Glsl::Mat4 { glm::value_ptr( this->m_space.projection ) } );
}

void Shape::reset_space_model()
{
    this->m_spaceModel = glm::mat4 { 1.f };
}

template < typename ArrayType >
static void bind_buffer_object( unsigned int const & bufferObjectID,
                                GLenum const & target,
                                std::vector< ArrayType > const & dataArray )
{
    ASSERTION( target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER,
               "Invalid target argument passed" );

    int const dataExpectedUsage { GL_STATIC_DRAW };

    glBindBuffer( target, bufferObjectID );
    glBufferData( target,
                  static_cast< int >( too::size_of( dataArray ) ),
                  too::to_c_style_array( dataArray ),
                  dataExpectedUsage );
}
