#include "load_shape.hpp"

#include <optional> // for optional
#include <stdint.h> // for intptr_t
#include <vector>   // for vector

#include <GLAD/glad.h> // for GLenum, GL_ARRAY_BUFFER, GL_ELEMENT_...

#include "graphics3D/shape.hpp" // for Shape, Shape::S_Data
#include "tools/assertion.hpp"  // for ASSERTION
#include "tools/tools.tpp"      // for size_of, to_c_style_array

namespace load_gl_shape
{
    static void objects_generation( Shape & shape );
    static void objects_binding( Shape const & shape );
    static void vertex_shader_attribution( Shape const & shape );
    static void unbind();
    template < typename ArrayType >
    static void bind_buffer_object(
        unsigned int bufferObjectID, GLenum target,
        std::vector< ArrayType > const & dataArray );

    void complete( Shape & shape )
    {
        objects_generation( shape );
        objects_binding( shape );
        vertex_shader_attribution( shape );
        unbind();
    }

    static void objects_generation( Shape & shape )
    {
        glGenVertexArrays( 1, &shape.get_VAO() );
        glGenBuffers( 1, &shape.get_VBO() );
        if ( shape.is_EBO_handled() )
        {
            glGenBuffers( 1, &shape.get_EBO() );
        }
    }

    static void objects_binding( Shape const & shape )
    {
        glBindVertexArray( shape.get_VAO() );

        GLenum const bufferTarget { GL_ARRAY_BUFFER };
        bind_buffer_object( shape.get_VBO(),
                            bufferTarget,
                            shape.get_data().vertices );

        if ( shape.is_EBO_handled() )
        {
            GLenum const elementTarget { GL_ELEMENT_ARRAY_BUFFER };
            bind_buffer_object( shape.get_EBO(),
                                elementTarget,
                                shape.get_data().indices.value() );
        }
    }

    static void vertex_shader_attribution( Shape const & shape )
    {
        int const valueType { GL_FLOAT };
        int const hasDataToBeNormalised { GL_FALSE };
        int const dataAttributeRealSize { static_cast< int >(
            shape.get_data().get_data_per_point_sum() * sizeof( float ) ) };

        unsigned int vectorSizeCounter { 0u };

        for ( unsigned int location { 0u };
              location < shape.get_data().dataPerPoint.size();
              ++location )
        {
            void * offsetStart { reinterpret_cast< void * >(
                static_cast< intptr_t >( vectorSizeCounter
                                         * sizeof( float ) ) ) };

            glVertexAttribPointer(
                location,
                static_cast< int >( shape.get_data().dataPerPoint[location] ),
                valueType,
                hasDataToBeNormalised,
                dataAttributeRealSize,
                offsetStart );
            glEnableVertexAttribArray( location );

            vectorSizeCounter += shape.get_data().dataPerPoint[location];
        }
    }

    static void unbind()
    {
        // Unbind
        glBindBuffer( GL_ARRAY_BUFFER, 0 ); // je ne sais pas ce que ça fait
        // Unbind VAO
        glBindVertexArray( 0 );
        // Unbind EBO
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    template < typename ArrayType >
    static void bind_buffer_object( unsigned int bufferObjectID, GLenum target,
                                    std::vector< ArrayType > const & dataArray )
    {
        ASSERTION(
            target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER,
            "Invalid target argument passed" );

        int const dataExpectedUsage { GL_STATIC_DRAW };

        glBindBuffer( target, bufferObjectID );
        glBufferData( target,
                      static_cast< int >( tools::size_of( dataArray ) ),
                      tools::to_c_style_array( dataArray ),
                      dataExpectedUsage );
    }

} // namespace load_gl_shape