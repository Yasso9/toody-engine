#include "shape_builder.hpp"

#include "graphics3D/shape.hpp"  // for Shape
#include "tools/assertion.hpp"   // for ASSERTION
#include "tools/tools.tpp"       // for size_of, to_c_style_array

ShapeBuilder::ShapeBuilder( Shape & shape ) : m_shape { shape } {}

void ShapeBuilder::construct()
{
    this->objects_generation();
    this->objects_binding();
    this->vertex_shader_attribution();
    this->unbind();
}

void ShapeBuilder::destroy()
{
    // We delete all the buffers and the objects generated
    glDeleteVertexArrays( 1, &m_shape.get_VAO() );
    glDeleteBuffers( 1, &m_shape.get_VBO() );
    if ( m_shape.is_EBO_handled() )
    {
        glDeleteBuffers( 1, &m_shape.get_EBO() );
    }
}

void ShapeBuilder::objects_generation()
{
    // We generate the VAO
    glGenVertexArrays( 1, &m_shape.get_VAO() );
    // We generate the VBO
    glGenBuffers( 1, &m_shape.get_VBO() );
    // We generate the EBO if needed
    if ( m_shape.is_EBO_handled() )
    {
        glGenBuffers( 1, &m_shape.get_EBO() );
    }
}

void ShapeBuilder::objects_binding()
{
    // Bind the VAO
    glBindVertexArray( m_shape.get_VAO() );
    // Bind the VBO
    glBindBuffer( GL_ARRAY_BUFFER, m_shape.get_VBO() );
    // Bind the EBO
    if ( m_shape.is_EBO_handled() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_shape.get_EBO() );
    }

    // We send the data to the GPU
    this->bind_buffer_object( m_shape.get_VBO(), GL_ARRAY_BUFFER,
                              m_shape.get_data().vertices );

    if ( m_shape.is_EBO_handled() )
    {
        this->bind_buffer_object( m_shape.get_EBO(), GL_ELEMENT_ARRAY_BUFFER,
                                  m_shape.get_data().indices.value() );
    }
}

void ShapeBuilder::vertex_shader_attribution()
{
    int const          valueType { GL_FLOAT };
    // Normalised means that the data is normalised to the range [-1, 1]
    int const          isNormalised { GL_FALSE };
    // Stride is the number of bytes between each vertex
    int const          stride { 0 };
    // Offset is the number of bytes of the first vertex
    void const * const offset { nullptr };

    int const positionIndex { 0 };
    int const positionSize { 3 };
    glVertexAttribPointer( positionIndex, positionSize, valueType, isNormalised,
                           stride, offset );
    glEnableVertexAttribArray( positionIndex );

    int const textureIndex { 1 };
    int const textureSize { 2 };
    glVertexAttribPointer( textureIndex, textureSize, valueType, isNormalised,
                           stride, offset );
    glEnableVertexAttribArray( textureIndex );

    int const normalIndex { 2 };
    int const normalSize { 3 };
    glVertexAttribPointer( normalIndex, normalSize, valueType, isNormalised,
                           stride, offset );
    glEnableVertexAttribArray( normalIndex );
}

void ShapeBuilder::unbind()
{
    // Unbind the VAO
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    // Unbind the VBO
    glBindVertexArray( 0 );
    // Unbind the EBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

template< typename T >
void ShapeBuilder::bind_buffer_object( unsigned int             bufferObjectID,
                                       GLenum                   target,
                                       std::vector< T > const & dataArray )
{
    ASSERTION( target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER,
               "Invalid target argument passed" );

    int const dataExpectedUsage { GL_STATIC_DRAW };

    glBindBuffer( target, bufferObjectID );
    glBufferData( target, static_cast< int >( tools::size_of( dataArray ) ),
                  tools::to_c_style_array( dataArray ), dataExpectedUsage );
}