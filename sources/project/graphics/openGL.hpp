#pragma once

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp> // vector rotation
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

#include <ostream>

namespace gl
{
    struct SpaceMatrix
    {
        glm::mat4 projection { 1.f };
        glm::mat4 view { 1.f };
        glm::mat4 model { 1.f };
    };

    void initialize();
    void clear_window();
    void set_wireframe();
    void remove_wireframe();
} // namespace gl

std::ostream & operator<<( std::ostream & stream, glm::vec3 const & vector3 );