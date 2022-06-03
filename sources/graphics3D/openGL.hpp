#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#if defined( __clang__ )
    #pragma GCC diagnostic ignored "-Wlanguage-extension-token"
#endif

#include <GLAD/glad.h>
#include <SFML/OpenGL.hpp>

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#if defined( __clang__ )
    #pragma GCC diagnostic ignored "-Wnested-anon-types"
    #pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#if defined( __GNUC__ )
    #pragma GCC diagnostic ignored "-Weffc++"
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

#include <GLM/ext/matrix_clip_space.hpp> // glm::perspective
#include <GLM/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <GLM/ext/scalar_constants.hpp> // glm::pi
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/rotate_vector.hpp> // vector rotation
#include <GLM/mat4x4.hpp> // glm::mat4
#include <GLM/vec3.hpp> // glm::vec3
#include <GLM/vec4.hpp> // glm::vec4

#pragma GCC diagnostic pop

#include <ostream>

#include "graphics2D/sfml.hpp"

namespace gl
{
    struct SpaceMatrix
    {
        glm::mat4 projection { 1.f };
        glm::mat4 view { 1.f };
        glm::mat4 model { 1.f };
    };

    /// @brief Initialize the OpenGL context and functions
    void initialize( unsigned int const & width, unsigned int const & height );
    void clear_window( sf::Color const & backgroundColor );
    void set_wireframe();
    void remove_wireframe();
    void check_error();
} // namespace gl

std::ostream & operator<<( std::ostream & stream, glm::vec3 const & vector3 );