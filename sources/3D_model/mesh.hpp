#pragma once

#include <vector>

#include "graphics/openGL.hpp"
#include "graphics/sfml.hpp"
#include "main/window.hpp"
#include "tools/string.hpp"

#define MAX_BONE_INFLUENCE 4

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
    int boneIDs[MAX_BONE_INFLUENCE];
    /// @brief weights from each bone
    float weights[MAX_BONE_INFLUENCE];
};

struct S_Texture
{
    unsigned int id;
    // sf::Texture texture {};
    std::string type;
    std::string path;
};

class Mesh
{
  public:
    std::vector< S_Vertex > vertices;
    std::vector< unsigned int > indices;
    std::vector< S_Texture > textures;
    unsigned int VAO;

    Mesh( std::vector< S_Vertex > vertices, std::vector< unsigned int > indices,
          std::vector< S_Texture > textures );

    void draw( sf::Shader const & shader ) const;

  private:
    unsigned int VBO, EBO;

    void setupMesh();
};