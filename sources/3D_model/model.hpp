#pragma once

#include "3D_model/assimp.hpp"
#include "3D_model/mesh.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Model
{
  public:
    /** @brief Model data.
     * Stores all the textures loaded so far,
     * optimization to make sure textures aren't loaded more than once.
     */
    std::vector<S_Texture> m_texturesLoaded;
    /// @brief All the meshes that the model contains
    std::vector<Mesh> m_meshes;

    Model( std::string const & filePath3DModel )
      : m_texturesLoaded(), m_meshes()
    {
        this->load_model( filePath3DModel );
    }

    // draws the model, and thus all its meshes
    void draw( sf::Shader const & shader ) const
    {
        for ( auto const & mesh : this->m_meshes )
        {
            mesh.draw( shader );
        }
    }

  private:
    /**
     * @brief Loads a model with supported ASSIMP extensions from file
     * and stores the resulting meshes in the meshes vector.
     */
    void load_model( std::string const & path )
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        unsigned int const importerFlags { aiProcess_Triangulate
                                           | aiProcess_GenSmoothNormals
                                           | aiProcess_FlipUVs
                                           | aiProcess_CalcTangentSpace };
        aiScene const * scene = importer.ReadFile( path, importerFlags );
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
     * @brief processes a node in a recursive fashion.
     * Processes each individual mesh located at the node and repeats
     * this process on its children nodes (if any).
     */
    void process_node( aiNode * node, const aiScene * scene )
    {
        // parse each mesh located at the current node
        for ( unsigned int i_mesh { 0u }; i_mesh < node->mNumMeshes; ++i_mesh )
        {
            unsigned int meshKey { node->mMeshes[i_mesh] };
            aiMesh * mesh = scene->mMeshes[meshKey];
            this->m_meshes.push_back( this->process_mesh( mesh, scene ) );
        }

        // after we've processed all of the meshes (if any),
        // we then recursively process each of the meshes of children nodes
        for ( unsigned int i_nodeChild { 0u }; i_nodeChild < node->mNumChildren;
              ++i_nodeChild )
        {
            this->process_node( node->mChildren[i_nodeChild], scene );
        }
    }

    Mesh process_mesh( aiMesh * mesh, const aiScene * scene )
    {
        std::vector<S_Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<S_Texture> textures;

        // walk through each of the mesh's vertices
        for ( unsigned int i_vertices { 0u }; i_vertices < mesh->mNumVertices;
              i_vertices++ )
        {
            S_Vertex vertex;
            // we declare a placeholder vector since assimp uses its own vector class
            // that doesn't directly convert to glm's vec3 class so we transfer the data
            // to this placeholder glm::vec3 first.
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i_vertices].x;
            vector.y = mesh->mVertices[i_vertices].y;
            vector.z = mesh->mVertices[i_vertices].z;
            vertex.position = vector;
            // normals
            if ( mesh->HasNormals() )
            {
                vector.x = mesh->mNormals[i_vertices].x;
                vector.y = mesh->mNormals[i_vertices].y;
                vector.z = mesh->mNormals[i_vertices].z;
                vertex.normal = vector;
            }
            // texture coordinates
            // does the mesh contain texture coordinates?
            if ( mesh->mTextureCoords[0] )
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i_vertices].x;
                vec.y = mesh->mTextureCoords[0][i_vertices].y;
                vertex.textureCoordinates = vec;
                // tangent
                vector.x = mesh->mTangents[i_vertices].x;
                vector.y = mesh->mTangents[i_vertices].y;
                vector.z = mesh->mTangents[i_vertices].z;
                vertex.tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i_vertices].x;
                vector.y = mesh->mBitangents[i_vertices].y;
                vector.z = mesh->mBitangents[i_vertices].z;
                vertex.bitangent = vector;
            }
            else
            {
                vertex.textureCoordinates = glm::vec2( 0.0f, 0.0f );
            }

            vertices.push_back( vertex );
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for ( unsigned int i = 0; i < mesh->mNumFaces; i++ )
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for ( unsigned int j = 0; j < face.mNumIndices; j++ )
            {
                indices.push_back( face.mIndices[j] );
            }
        }
        // process materials
        aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<S_Texture> diffuseMaps { this->load_material_textures(
            material,
            aiTextureType_DIFFUSE,
            "texture_diffuse" ) };
        textures.insert( textures.end(),
                         diffuseMaps.begin(),
                         diffuseMaps.end() );
        // 2. specular maps
        std::vector<S_Texture> specularMaps { this->load_material_textures(
            material,
            aiTextureType_SPECULAR,
            "texture_specular" ) };
        textures.insert( textures.end(),
                         specularMaps.begin(),
                         specularMaps.end() );
        // 3. normal maps
        std::vector<S_Texture> normalMaps { this->load_material_textures(
            material,
            aiTextureType_HEIGHT,
            "texture_normal" ) };
        textures.insert( textures.end(), normalMaps.begin(), normalMaps.end() );
        // 4. height maps
        std::vector<S_Texture> heightMaps { this->load_material_textures(
            material,
            aiTextureType_AMBIENT,
            "texture_height" ) };
        textures.insert( textures.end(), heightMaps.begin(), heightMaps.end() );

        // return a mesh object created from the extracted mesh data
        return Mesh( vertices, indices, textures );
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<S_Texture> load_material_textures( aiMaterial * material,
                                                   aiTextureType type,
                                                   std::string typeName )
    {
        std::vector<S_Texture> textures;

        for ( unsigned int i { 0u }; i < material->GetTextureCount( type );
              i++ )
        {
            aiString str;
            material->GetTexture( type, i, &str );
            // check if texture was loaded before and if so,
            // continue to next iteration: skip loading a new texture
            bool skip { false };
            for ( unsigned int j { 0u }; j < m_texturesLoaded.size(); j++ )
            {
                if ( std::strcmp( m_texturesLoaded[j].path.data(), str.C_Str() )
                     == 0 )
                {
                    textures.push_back( m_texturesLoaded[j] );
                    // a texture with the same filepath has already been loaded,
                    // continue to next one. (optimization)
                    skip = true;
                    break;
                }
            }

            if ( ! skip )
            {
                continue;
            }
            // if texture hasn't been loaded already, load it
            S_Texture texture {};

            sf::Texture sfmlTexture {};
            sfmlTexture.loadFromFile( str.C_Str() );
            texture.id = sfmlTexture.getNativeHandle();

            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back( texture );
            // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            m_texturesLoaded.push_back( texture );
        }

        return textures;
    }
};