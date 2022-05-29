#pragma once

#include <iostream>
#include <vector>

#include "3D_model/assimp.hpp"
#include "graphics/openGL.hpp"
#include "graphics/texture.hpp"
#include "graphics2D/sfml.hpp"
#include "main/window.hpp"
#include "tools/string.hpp"

constexpr unsigned int const maxBoneInfluence { 4u };

struct S_Vertex
{
    /// @brief position
    glm::vec3 position;
    /// @brief normal
    glm::vec3 normal;
    /// @brief texCoords
    glm::vec2 textureCoordinates;
    /// @brief tangent
    glm::vec3 tangent;
    /// @brief bitangent
    glm::vec3 bitangent;
    /// @brief bone indexes which will influence this vertex
    int boneIDs[maxBoneInfluence];
    /// @brief weights from each bone
    float weights[maxBoneInfluence];
};

class Mesh
{
  public:
    Mesh( std::vector< S_Vertex > const & vertices,
          std::vector< unsigned int > const & indices,
          std::vector< std::string > const & textures );

    void update( sf::Shader & shader,
                 std::map< std::string, Texture > const & textureLoaded );

    void draw() const;

  private:
    std::vector< S_Vertex > const m_vertices;
    std::vector< unsigned int > const m_indices;
    std::vector< std::string > const m_textures;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void generate();
};