#pragma once

#include <optional>

#include "3D_model/assimp.hpp"
#include "3D_model/mesh.hpp"
#include "graphics/texture.hpp"
#include "graphics/transformable.hpp"
#include "tools/tools.hpp"

class Model final : public Transformable
{
  public:
    Model( std::string const & filePathModel );

  private:
    /** @brief Model data.
     * Stores all the textures loaded so far,
     * optimization to make sure textures aren't loaded more than once.
     */
    std::map< std::string, Texture > m_texturesLoaded;
    /// @brief All the meshes that the model contains
    std::vector< Mesh > m_meshes;
    /// @brief Path of the model object file
    std::string const m_filePath;

    void update_intra() override;
    void draw_intra() const override;

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

    /**
     * @brief checks all material textures of a given type and loads
     *        the textures if they're not loaded yet.
     *        the required info is returned as a Texture struct.
    */
    std::vector< std::string > load_material_textures(
        aiMaterial const & material, Texture::E_Type const & textureType );

    /**
     * @brief Return the texture from the texture path.
     *        If the texture doesn't exist, return nothing
     */
    std::optional< Texture > get_texture_loaded(
        std::string const & texturePath ) const;

    std::vector< S_Vertex > load_vertices( aiMesh const & mesh );
    std::vector< unsigned int > load_indices( aiMesh const & mesh );
    std::vector< std::string > load_textures( aiMaterial const & material );
};