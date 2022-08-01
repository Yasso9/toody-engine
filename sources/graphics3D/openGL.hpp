#pragma once

#include <cstddef> // for size_t
#include <ostream> // for ostream

#include <GLAD/glad.h>                 // for GLenum
#include <GLM/ext/matrix_float4x4.hpp> // for mat4
#include <GLM/ext/vector_float3.hpp>   // for vec3

namespace sf
{
    class Color;
} // namespace sf

namespace gl
{
    struct S_SpaceMatrix
    {
        glm::mat4 projection { 1.f };
        glm::mat4 view { 1.f };
        glm::mat4 model { 1.f };
    };

    void draw_elements( unsigned int vertexArrayObject, GLenum primitiveType,
                        GLenum dataType, std::size_t elementsSize );
    void draw_arrays( unsigned int vertexArrayObject, GLenum primitiveType,
                      unsigned int arraySize );

    /// @brief Initialize the OpenGL context and functions
    void initialize( unsigned int const & width, unsigned int const & height );
    void clear_window( sf::Color const & backgroundColor );
    void set_wireframe();
    void remove_wireframe();
    void check_error();
} // namespace gl

std::ostream & operator<<( std::ostream & stream, glm::vec3 const & vector3 );