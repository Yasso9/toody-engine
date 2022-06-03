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
unsigned int Shape::Data::get_number_of_element() const
{
    return static_cast< unsigned int >( this->vertices.size() )
           / this->get_data_per_point_sum();
}

Shape::Shape()
  : Transformable( tools::get_path::shaders( "shape_shader.vert"s ),
                   tools::get_path::shaders( "shape_shader.frag"s ) ),
    m_textureA(),
    m_textureB(),
    m_vertexArrayObject(),
    m_vertexBufferObject(),
    m_elementBufferObject(),
    m_data()
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

void Shape::update_intra()
{
    this->m_shader.setUniform( "my_textureA", this->m_textureA );
    this->m_shader.setUniform( "my_textureB", this->m_textureB );
}

void Shape::draw_intra() const
{
    GLenum const primitiveType { GL_TRIANGLES };
    if ( this->is_element_buffer_set() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_elementBufferObject );
        GLenum const dataType { GL_UNSIGNED_INT };
        Window::get_instance().gl_draw_elements( this->m_vertexArrayObject,
                                                 primitiveType,
                                                 dataType,
                                                 this->m_data.vertices.size() );
    }
    else
    {
        Window::get_instance().gl_draw_arrays(
            this->m_vertexArrayObject,
            primitiveType,
            this->m_data.get_number_of_element() );
    }
}

void Shape::load_textures_and_shaders()
{
    bool textureLoad { true };
    textureLoad &= this->m_textureA.loadFromFile(
        tools::get_path::resources( "wall.jpg"s ) );
    textureLoad &= this->m_textureA.generateMipmap();
    if ( ! textureLoad )
    {
        throw std::runtime_error { "Cannot load texture"s };
    }

    textureLoad &= this->m_textureB.loadFromFile(
        tools::get_path::resources( "town_hall.png"s ) );
    textureLoad &= this->m_textureB.generateMipmap();
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
                  static_cast< int >( tools::size_of( dataArray ) ),
                  tools::to_c_style_array( dataArray ),
                  dataExpectedUsage );
}
