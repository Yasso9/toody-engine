#include "shape.hpp"

#include <iostream>
#include <numeric>

#include "graphics3D/load_shape.hpp"
#include "main/resources.hpp"
#include "main/window.hpp"
#include "tools/assertion.hpp"
#include "tools/exceptions.hpp"
#include "tools/path.hpp"
#include "tools/tools.hpp"

unsigned int Shape::S_Data::get_data_per_point_sum() const
{
    return std::accumulate( this->dataPerPoint.begin(),
                            this->dataPerPoint.end(),
                            0u );
}
unsigned int Shape::S_Data::get_number_of_element() const
{
    return static_cast< unsigned int >( this->vertices.size() )
           / this->get_data_per_point_sum();
}

Shape::Shape( S_Data const & data, Camera const & camera )
  : Transformable { camera,
                    resources::get_shader( "shape_shader.vert"s,
                                           "shape_shader.frag"s ) },
    m_textures { resources::get_texture( "wall.jpg" ) },
    m_vertexArrayObject {},
    m_vertexBufferObject {},
    m_elementBufferObject {},
    m_data { data }
{
    load_gl_shape::complete( *this );
}

Shape::~Shape()
{
    // We delete all the buffers and the objects generated
    glDeleteVertexArrays( 1, &this->m_vertexArrayObject );
    glDeleteBuffers( 1, &this->m_vertexBufferObject );

    if ( this->is_EBO_handled() )
    {
        glDeleteBuffers( 1, &this->m_elementBufferObject );
    }
}

void Shape::update_custom( float /* deltaTime */ )
{
    for ( auto const & texture : m_textures )
    {
        this->get_shader().setUniform( "my_texture", texture );
    }
}
void Shape::render_custom( Window const & window ) const
{
    GLenum const primitiveType { GL_TRIANGLES };

    if ( this->is_EBO_handled() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject );
        GLenum const dataType { GL_UNSIGNED_INT };
        window.gl_draw_elements( m_vertexArrayObject,
                                 primitiveType,
                                 dataType,
                                 m_data.vertices.size() );
    }
    else
    {
        window.gl_draw_arrays( m_vertexArrayObject,
                               primitiveType,
                               m_data.get_number_of_element() );
    }
}

unsigned int Shape::get_VAO() const
{
    return const_cast< Shape * >( this )->get_VAO();
}
unsigned int & Shape::get_VAO()
{
    return m_vertexArrayObject;
}

unsigned int Shape::get_VBO() const
{
    return const_cast< Shape * >( this )->get_VBO();
}
unsigned int & Shape::get_VBO()
{
    return m_vertexBufferObject;
}

unsigned int Shape::get_EBO() const
{
    return const_cast< Shape * >( this )->get_EBO();
}
unsigned int & Shape::get_EBO()
{
    return m_elementBufferObject;
}

Shape::S_Data const & Shape::get_data() const
{
    return m_data;
}

bool Shape::is_EBO_handled() const
{
    return this->m_data.indices.has_value();
}
