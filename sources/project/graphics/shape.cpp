#include "shape.hpp"

#include "tools/resources.hpp"

#include "iostream"

Shape::~Shape()
{
    // We delete all the buffers and object generated
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

    this->m_shaders.load( too::get_shaders_path() + "/shader.vert"s,
                          too::get_shaders_path() + "/shader.frag"s );

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
    this->m_shaders.use();

    glBindVertexArray( this->m_vertexArrayObject );

    this->transform();

    int const primitiveType { GL_TRIANGLES };
    int const vectorSize { static_cast<int>(
        this->m_vertices.size() / this->m_numberOfDataPerAttribute ) };
    if ( this->is_element_buffer_set() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_elementBufferObject );
        int const dataType { GL_UNSIGNED_INT };
        glDrawElements( primitiveType, vectorSize, dataType, 0 );
    }
    else
    {
        int const verticesBeginPosition { 0 };
        glDrawArrays( primitiveType, verticesBeginPosition, vectorSize );
    }

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
    unsigned int const location { 0u };
    unsigned int const vectorSize { 3u };
    int const valueType { GL_FLOAT };
    int const hasDataToBeNormalised { GL_FALSE };
    void * offsetStart { 0 };

    glVertexAttribPointer(
        location,
        vectorSize,
        valueType,
        hasDataToBeNormalised,
        static_cast<int>( this->m_numberOfDataPerAttribute * sizeof( float ) ),
        offsetStart );
    glEnableVertexAttribArray( location );
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

void Shape::transform() const
{
    static float count { 0.5f };

    int const numberOfMatrix { 1 };
    int const transposeMatrix { GL_FALSE };

    int modelLoc = this->m_shaders.get_uniform_location( "model"s );
    int viewLoc = this->m_shaders.get_uniform_location( "view"s );
    int projectionLoc = this->m_shaders.get_uniform_location( "projection"s );

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