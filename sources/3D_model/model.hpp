#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "3D_model/assimp.hpp"
#include "3D_model/mesh.hpp"
#include "graphics/texture.hpp"
#include "tools/tools.hpp"

inline glm::vec3 to_vector3( aiVector3D const & assimpVector3D )
{
    glm::vec3 glmVec3 {};

    glmVec3.x = assimpVector3D.x;
    glmVec3.y = assimpVector3D.y;
    glmVec3.z = assimpVector3D.z;

    return glmVec3;
}

inline glm::vec2 to_vector2( aiVector3D const & assimpVector3D )
{
    glm::vec2 glmVec2 {};

    glmVec2.x = assimpVector3D.x;
    glmVec2.y = assimpVector3D.y;

    return glmVec2;
}

class Model
{
  public:
    /** @brief Model data.
     * Stores all the textures loaded so far,
     * optimization to make sure textures aren't loaded more than once.
     */
    std::vector< S_Texture > m_texturesLoaded;
    /// @brief All the meshes that the model contains
    std::vector< Mesh > m_meshes;
    /// @brief space where the object is and where we can move it
    gl::SpaceMatrix m_space;
    glm::mat4 m_spaceModel;
    sf::Shader m_shader;
    std::vector< sf::Texture > sfmlTextures {};
    std::string m_filePathModel {};

    Model( std::string const & filePathModel ) : m_texturesLoaded(), m_meshes()
    {
        this->m_filePathModel = filePathModel;
        this->load_model();
        this->m_shader.loadFromFile(
            tools::get_path::shaders( "shader.vert"s ),
            tools::get_path::shaders( "shader.frag"s ) );
    }

    void translate( glm::vec3 const & tranlationVector )
    {
        glm::mat4 const translationMatrix {
            glm::translate( glm::mat4 { 1.f }, tranlationVector )
        };

        this->m_spaceModel *= translationMatrix;
    }
    void rotate( glm::vec3 const & rotationVector, float const & angle )
    {
        glm::mat4 const rotationMatrix { glm::rotate(
            glm::mat4 { 1.f },
            glm::radians( angle ),
            glm::vec3 { rotationVector } ) };

        this->m_spaceModel *= rotationMatrix;
    }
    void scale( glm::vec3 const & scaleVector )
    {
        glm::mat4 const scaleMatrix { glm::scale( glm::mat4 { 1.f },
                                                  glm::vec3 { scaleVector } ) };
        this->m_spaceModel *= scaleMatrix;
    }

    void update( glm::mat4 const & projection, glm::mat4 const & view )
    {
        this->m_space.projection = projection;
        this->m_space.view       = view;
        this->m_space.model      = this->m_spaceModel;

        // All the object transformation have been made, so we reset the matrix to identity
        this->reset_space_model();

        this->transform();
    }

    // draws the model, and thus all its meshes
    void draw() const
    {
        sf::Shader::bind( &this->m_shader );

        for ( Mesh const & mesh : this->m_meshes )
        {
            mesh.draw( this->m_shader );
        }

        sf::Shader::bind( NULL );
    }

  private:
    /**
     * @brief Loads a model with supported ASSIMP extensions from file
     * and stores the resulting meshes in the meshes vector.
     */
    void load_model()
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        unsigned int const importerFlags { aiProcess_Triangulate
                                           | aiProcess_GenSmoothNormals
                                           | aiProcess_FlipUVs
                                           | aiProcess_CalcTangentSpace };
        aiScene const * scene =
            importer.ReadFile( this->m_filePathModel, importerFlags );
        // check for errors
        if ( ! scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
             || ! scene->mRootNode )
        {
            throw std::runtime_error { importer.GetErrorString() };
        }

        // process ASSIMP's root node recursively
        this->process_node( scene->mRootNode, scene );
    }

    /**
     * @brief Processes a node in a recursive way.
     * Processes each individual mesh located at the current node and repeats
     * this process on its children nodes (if any).
     */
    void process_node( aiNode const * node, aiScene const * scene )
    {
        // Parse each mesh located at the current node
        for ( unsigned int i_mesh { 0u }; i_mesh < node->mNumMeshes; ++i_mesh )
        {
            unsigned int const meshKey { node->mMeshes[i_mesh] };
            aiMesh const * currentMesh { scene->mMeshes[meshKey] };
            Mesh const & processedMesh { this->process_mesh( currentMesh,
                                                             scene ) };
            this->m_meshes.push_back( processedMesh );
        }

        // After we've processed all of the meshes (if any),
        // We then recursively process each of the meshes of children nodes
        for ( unsigned int i_nodeChild { 0u }; i_nodeChild < node->mNumChildren;
              ++i_nodeChild )
        {
            this->process_node( node->mChildren[i_nodeChild], scene );
        }
    }

    Mesh process_mesh( aiMesh const * mesh, aiScene const * scene )
    {
        std::vector< S_Vertex > vertices {};
        std::vector< unsigned int > indices {};
        std::vector< S_Texture > textures {};

        // walk through each of the mesh's vertices
        for ( unsigned int i_vertices { 0u }; i_vertices < mesh->mNumVertices;
              ++i_vertices )
        {
            S_Vertex vertex {};
            // we declare a placeholder vector since assimp uses its own vector class
            // that doesn't directly convert to glm's vec3 class so we transfer the data
            // to this placeholder glm::vec3 first.
            vertex.position = to_vector3( mesh->mVertices[i_vertices] );

            if ( mesh->HasNormals() )
            {
                vertex.normal = to_vector3( mesh->mNormals[i_vertices] );
            }
            // Does the mesh contain texture coordinates?
            if ( mesh->mTextureCoords[0] )
            {
                // TYPO a vertex can contain up to 8 different texture coordinates.
                // We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates
                // so we always take the first set (0).
                vertex.textureCoordinates =
                    to_vector2( mesh->mTextureCoords[0][i_vertices] );
                // tangent
                vertex.tangent = to_vector3( mesh->mTangents[i_vertices] );
                // bitangent
                vertex.bitangent = to_vector3( mesh->mBitangents[i_vertices] );
            }
            else
            {
                vertex.textureCoordinates = glm::vec2 { 0.0f, 0.0f };
            }

            vertices.push_back( vertex );
        }
        // now wak through each of the mesh's faces
        // (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for ( unsigned int i_face { 0u }; i_face < mesh->mNumFaces; ++i_face )
        {
            aiFace const face = mesh->mFaces[i_face];
            // retrieve all indices of the face and store them in the indices vector
            for ( unsigned int i_indice { 0u }; i_indice < face.mNumIndices;
                  ++i_indice )
            {
                indices.push_back( face.mIndices[i_indice] );
            }
        }
        // process materials
        aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders.
        // Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        std::vector< S_Texture > const diffuseMaps {
            this->load_material_textures( material,
                                          aiTextureType::aiTextureType_DIFFUSE,
                                          "texture_diffuse" )
        };
        std::vector< S_Texture > const specularMaps {
            this->load_material_textures( material,
                                          aiTextureType::aiTextureType_SPECULAR,
                                          "texture_specular" )
        };
        std::vector< S_Texture > const normalMaps {
            this->load_material_textures( material,
                                          aiTextureType::aiTextureType_HEIGHT,
                                          "texture_normal" )
        };
        std::vector< S_Texture > const heightMaps {
            this->load_material_textures( material,
                                          aiTextureType::aiTextureType_AMBIENT,
                                          "texture_height" )
        };

        textures.insert( textures.end(),
                         diffuseMaps.begin(),
                         diffuseMaps.end() );
        textures.insert( textures.end(),
                         specularMaps.begin(),
                         specularMaps.end() );
        textures.insert( textures.end(), normalMaps.begin(), normalMaps.end() );
        textures.insert( textures.end(), heightMaps.begin(), heightMaps.end() );

        // return a mesh object created from the extracted mesh data
        return Mesh( vertices, indices, textures );
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector< S_Texture > load_material_textures( aiMaterial * material,
                                                     aiTextureType type,
                                                     std::string typeName )
    {
        std::vector< S_Texture > textures;
        for ( unsigned int i = 0; i < material->GetTextureCount( type ); i++ )
        {
            aiString str;
            material->GetTexture( type, i, &str );
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for ( unsigned int j = 0; j < m_texturesLoaded.size(); j++ )
            {
                if ( std::strcmp( m_texturesLoaded[j].path.data(), str.C_Str() )
                     == 0 )
                {
                    textures.push_back( m_texturesLoaded[j] );
                    skip =
                        true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if ( ! skip )
            { // if texture hasn't been loaded already, load it
                S_Texture texture;
                // if texture hasn't been loaded already, load it
                std::string directory { this->m_filePathModel.substr(
                    0,
                    this->m_filePathModel.find_last_of( '/' ) + 1 ) };
                // std::string texturePath { directory + str.C_Str() };
                // std::cout << "texture path : " << texturePath.c_str()
                //           << std::endl;
                // sfmlTextures.push_back( sf::Texture {} );
                // bool loaded =
                //     sfmlTextures.back().loadFromFile( texturePath.c_str() );
                // loaded &= sfmlTextures.back().generateMipmap();
                // if ( ! loaded )
                // {
                //     throw std::runtime_error {
                //         "texture not loaded correctly"s
                //     };
                // }
                texture.id = TextureFromFile( str.C_Str(), directory );
                // texture.id = sfmlTextures.back().getNativeHandle();
                // texture.texture = sfmlTextures.back();
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back( texture );
                this->m_texturesLoaded.push_back(
                    texture ); // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    void transform()
    {
        this->m_shader.setUniform(
            "model"s,
            sf::Glsl::Mat4 { glm::value_ptr( this->m_space.model ) } );
        this->m_shader.setUniform(
            "view"s,
            sf::Glsl::Mat4 { glm::value_ptr( this->m_space.view ) } );
        this->m_shader.setUniform(
            "projection"s,
            sf::Glsl::Mat4 { glm::value_ptr( this->m_space.projection ) } );
    }

    void reset_space_model() { this->m_spaceModel = glm::mat4 { 1.f }; }
};