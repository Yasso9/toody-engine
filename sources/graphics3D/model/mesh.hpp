#pragma once

#include <map>     // for map
#include <string>  // for string
#include <vector>  // for vector

#include <glm/detail/type_vec2.hpp>   // for vec
#include <glm/detail/type_vec3.hpp>   // for vec
#include <glm/ext/vector_float2.hpp>  // for vec2
#include <glm/ext/vector_float3.hpp>  // for vec3

class Texture;

namespace sf
{
    class Shader;
}  // namespace sf

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
    Mesh( std::vector< S_Vertex > const &     vertices,
          std::vector< unsigned int > const & indices,
          std::vector< std::string > const &  textures );

    void update ( sf::Shader &                             shader,
                  std::map< std::string, Texture > const & textureLoaded );

    void draw () const;

  private:
    std::vector< S_Vertex > const     m_vertices;
    std::vector< unsigned int > const m_indices;
    std::vector< std::string > const  m_textures;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void generate ();
};
