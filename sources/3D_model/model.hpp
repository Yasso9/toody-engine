#pragma once

#include <optional>

#include "3D_model/assimp.hpp"
#include "3D_model/mesh.hpp"
#include "graphics/texture.hpp"
#include "tools/tools.hpp"

class Model
{
  public:
    Model( std::string const & filePathModel );

    void translate( glm::vec3 const & tranlationVector );
    void rotate( glm::vec3 const & rotationVector, float const & angle );
    void scale( glm::vec3 const & scaleVector );

    void update( glm::mat4 const & projection, glm::mat4 const & view );

    void draw() const;

  private:
    /// @brief shader to activate when the object is loaded
    sf::Shader m_shader;
    /** @brief Model data.
     * Stores all the textures loaded so far,
     * optimization to make sure textures aren't loaded more than once.
     */
    std::vector< S_Texture > m_texturesLoaded;
    /// @brief All the meshes that the model contains
    std::vector< Mesh > m_meshes;
    /// @brief Space where the object is and where we can move it
    gl::SpaceMatrix m_space;
    glm::mat4 m_spaceModel;
    std::vector< sf::Texture > sfmlTextures {};
    std::string const m_filePathModel;

    /**
     * @brief Loads a model with supported ASSIMP extensions from file
     * and stores the resulting meshes in the meshes vector.
     */
    void load_model();

    /**
     * @brief Processes a node in a recursive way.
     * Processes each individual mesh located at the current node and repeats
     * this process on its children nodes (if any).
     */
    void process_node( aiNode const & node, aiScene const & scene );

    Mesh process_mesh( aiMesh const & mesh, aiScene const & scene );

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector< S_Texture > load_material_textures(
        aiMaterial const & material, aiTextureType const & type,
        std::string const & typeName );

    /**
     * @brief Return the texture from the texture path.
     *        If the texture doesn't exist, return nothing
     */
    std::optional< S_Texture > get_texture_loaded(
        std::string const & texturePath ) const;

    void transform();
    void reset_space_model();
};