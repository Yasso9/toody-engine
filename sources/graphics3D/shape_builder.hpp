#pragma once

#include <vector>  // for vector

#include <glad/glad.h>  // for GLenum

class Shape;

class ShapeBuilder
{
    Shape & m_shape;

  public:
    ShapeBuilder( Shape & shape );
    virtual ~ShapeBuilder() = default;

    void construct ();
    void destroy ();

  private:
    void objects_generation ();
    void objects_binding ();
    void vertex_shader_attribution ();
    void unbind ();

    template< typename T >
    void bind_buffer_object ( unsigned int bufferObjectID, GLenum target,
                              std::vector< T > const & dataArray );
};