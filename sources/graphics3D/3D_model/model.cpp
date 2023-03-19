#include "model.hpp"

#include <algorithm>  // for max
#include <iostream>   // for operator<<, basic_ostream, endl

#include <GLM/detail/type_vec2.hpp>   // for vec<>::(anonymous)
#include <GLM/detail/type_vec3.hpp>   // for vec<>::(anonymous)
#include <GLM/ext/vector_float2.hpp>  // for vec2
#include <GLM/ext/vector_float3.hpp>  // for vec3
#include <GLM/fwd.hpp>                // for vec2
#include <assimp/Importer.hpp>        // for Importer
#include <assimp/material.h>          // for aiMaterial
#include <assimp/mesh.h>              // for aiMesh, aiFace
#include <assimp/postprocess.h>       // for aiProcess_CalcTangentSpace, aiP...
#include <assimp/scene.h>             // for aiScene, aiNode, AI_SCENE_FLAGS...
#include <assimp/types.h>             // for aiString
#include <assimp/vector3.h>           // for aiVector3D

#include "game/resources.hpp"  // for get_shader
#include "tools/path.hpp"      // for get_folder, E_Folder, E_Folder:...
#include "tools/timer.hpp"     // for Timer
#include "tools/traces.hpp"

static glm::vec3 to_vector3 ( aiVector3D const & assimpVector3D );
static glm::vec2 to_vector2 ( aiVector3D const & assimpVector3D );

Model::Model( Camera const & camera, std::string const & filePathModel )
  : Transformable( camera,
                   resources::get_shader( "shader.vert", "shader.frag" ) ),
    m_texturesLoaded {},
    m_meshes {},
    m_filePath( path::get_folder( path::E_Folder::Resources ) / filePathModel )
{
    Timer timer { "Model creation (" + filePathModel + ')' };

    Trace::Info( "Loading Model - " + filePathModel );
    this->load_model();
}

void Model::update( UpdateContext /* context */ )
{
    for ( Mesh & mesh : this->m_meshes )
    {
        mesh.update( this->get_shader(), this->m_texturesLoaded );
    }
}

void Model::render( RenderContext /* context */ ) const
{
    for ( Mesh const & mesh : this->m_meshes )
    {
        mesh.draw();
    }
}

void Model::load_model()
{
    Assimp::Importer   importer;
    unsigned int const importerFlags {
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
        | aiProcess_CalcTangentSpace };
    aiScene const * scene {
        importer.ReadFile( this->m_filePath.string(), importerFlags ) };
    // check for when reading file
    if ( ! scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
         || ! scene->mRootNode )
    {
        Trace::FileIssue( m_filePath, std::string { "Assimp - " }
                                          + importer.GetErrorString() );
    }

    // process ASSIMP's root node recursively
    this->process_node( *( scene->mRootNode ), *scene );
}

void Model::process_node( aiNode const & node, aiScene const & scene )
{
    // Process each mesh located at the current node and add then to the
    // m_meshes vector
    for ( unsigned int i_mesh { 0u }; i_mesh < node.mNumMeshes; ++i_mesh )
    {
        unsigned int const meshKey { node.mMeshes[i_mesh] };
        aiMesh const &     currentMesh { *scene.mMeshes[meshKey] };
        Mesh const & processedMesh { this->process_mesh( currentMesh, scene ) };
        this->m_meshes.push_back( processedMesh );
    }

    // After we've processed all of the meshes (if any),
    // We then recursively process each of the meshes of children nodes
    for ( unsigned int i_nodeChild { 0u }; i_nodeChild < node.mNumChildren;
          ++i_nodeChild )
    {
        this->process_node( *node.mChildren[i_nodeChild], scene );
    }
}

Mesh Model::process_mesh( aiMesh const & mesh, aiScene const & scene )
{
    std::vector< S_Vertex > const & vertices { this->load_vertices( mesh ) };

    std::vector< unsigned int > const & indices { this->load_indices( mesh ) };

    // process materials
    aiMaterial const & material { *scene.mMaterials[mesh.mMaterialIndex] };
    std::vector< std::string > const & textures {
        this->load_textures( material ) };

    // return a mesh object created from the extracted mesh data
    return Mesh { vertices, indices, textures };
}

std::optional< Texture > Model::get_texture_loaded(
    std::string const & /* texturePath */ ) const
{
    // for ( Texture const & existingTexture : this->m_texturesLoaded )
    // {
    //     if ( existingTexture.get_path() == texturePath )
    //     {
    //         return std::make_optional< Texture >( existingTexture );
    //     }
    // }

    // No texture has been found
    return std::nullopt;
}

// checks all material textures of a given type and loads the textures if
// they're not loaded yet. the required info is returned as a Texture struct.
std::vector< std::string > Model::load_material_textures(
    aiMaterial const & material, Texture::E_Type const & textureType )
{
    std::vector< std::string > textures {};
    std::string const          directory { this->m_filePath.string().substr(
        0, this->m_filePath.string().find_last_of( '/' ) + 1 ) };

    for ( unsigned int i_textureToLoad { 0u };
          i_textureToLoad
          < material.GetTextureCount( Texture::to_assimp_type( textureType ) );
          ++i_textureToLoad )
    {
        // Getting the texture name
        aiString textureFileName;
        material.GetTexture( Texture::to_assimp_type( textureType ),
                             i_textureToLoad, &textureFileName );
        // std::cout << "Loading texture : "s << textureFileName.C_Str()
        //           << std::endl;
        std::string const texturePath { directory + textureFileName.C_Str() };

        // auto const textureLoaded { this->get_texture_loaded( texturePath ) };
        // The texture has already been loaded
        // if ( textureLoaded.has_value() )
        // {
        //     textures.push_back( textureLoaded.value().get_path() );
        //     continue;
        // }

        // The texture hasn't already been loaded, we load it
        // Texture const newTexture { texturePath, textureType };
        // Texture const newTexture { texturePath, textureType };
        textures.push_back( texturePath );

        if ( ! this->m_texturesLoaded.contains( texturePath ) )
        {
            // We save it in the loaded texture
            this->m_texturesLoaded.insert(
                { texturePath, Texture { texturePath, textureType } } );
        }
    }

    return textures;
}

std::vector< S_Vertex > Model::load_vertices( aiMesh const & mesh )
{
    std::vector< S_Vertex > vertices {};

    // Walk through each of the mesh's vertices
    for ( unsigned int i_vertice { 0u }; i_vertice < mesh.mNumVertices;
          ++i_vertice )
    {
        S_Vertex vertex {};
        // we declare a placeholder vector since assimp uses its own vector
        // class that doesn't directly convert to glm's vec3 class so we
        // transfer the data to this placeholder glm::vec3 first.
        vertex.position = to_vector3( mesh.mVertices[i_vertice] );

        if ( mesh.HasNormals() )
        {
            vertex.normal = to_vector3( mesh.mNormals[i_vertice] );
        }
        // Does the mesh contain texture coordinates?
        if ( mesh.mTextureCoords[0] )
        {
            /// @remarks a vertex can contain up to 8 different texture
            /// coordinates.
            // We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates
            // so we always take the first set (0).
            vertex.textureCoordinates =
                to_vector2( mesh.mTextureCoords[0][i_vertice] );
            // tangent
            vertex.tangent = to_vector3( mesh.mTangents[i_vertice] );
            // bitangent
            vertex.bitangent = to_vector3( mesh.mBitangents[i_vertice] );
        }
        else
        {
            vertex.textureCoordinates = glm::vec2 { 0.0f, 0.0f };
        }

        vertices.push_back( vertex );
    }

    return vertices;
}

std::vector< unsigned int > Model::load_indices( aiMesh const & mesh )
{
    std::vector< unsigned int > indices {};

    // now walk through each of the mesh's faces
    // (a face is a mesh is its triangle)
    // and retrieve the corresponding vertex indices.
    for ( unsigned int i_face { 0u }; i_face < mesh.mNumFaces; ++i_face )
    {
        aiFace const & face { mesh.mFaces[i_face] };
        // retrieve all indices of the face and store them in the indices vector
        for ( unsigned int i_indice { 0u }; i_indice < face.mNumIndices;
              ++i_indice )
        {
            indices.push_back( face.mIndices[i_indice] );
        }
    }

    return indices;
}

std::vector< std::string > Model::load_textures( aiMaterial const & material )
{
    // std::vector< std::string > textures {};

    std::vector< std::string > const & diffuseMaps {
        this->load_material_textures( material, Texture::E_Type::Diffuse ) };
    // std::vector< std::string > const & specularMaps {
    //     this->load_material_textures( material, Texture::E_Type::Specular )
    // };
    // std::vector< std::string > const & normalMaps {
    //     this->load_material_textures( material, Texture::E_Type::Normal )
    // };
    // std::vector< std::string > const & heightMaps {
    //     this->load_material_textures( material, Texture::E_Type::Height )
    // };

    // {
    //     Timer timer { "texture insert" };

    // textures.insert( textures.end(),
    //                  std::make_move_iterator( diffuseMaps.begin() ),
    //                  std::make_move_iterator( diffuseMaps.end() ) );
    // textures.insert( textures.end(),
    //                  std::make_move_iterator( specularMaps.begin() ),
    //                  std::make_move_iterator( specularMaps.end() ) );
    // textures.insert( textures.end(),
    //                  std::make_move_iterator( normalMaps.begin() ),
    //                  std::make_move_iterator( normalMaps.end() ) );
    // textures.insert( textures.end(),
    //                  std::make_move_iterator( heightMaps.begin() ),
    //                  std::make_move_iterator( heightMaps.end() ) ); }

    // std::cout << "number of texture : " << textures.size() << std::endl;

    return diffuseMaps;
}

static glm::vec3 to_vector3 ( aiVector3D const & assimpVector3D )
{
    glm::vec3 glmVec3 {};

    glmVec3.x = assimpVector3D.x;
    glmVec3.y = assimpVector3D.y;
    glmVec3.z = assimpVector3D.z;

    return glmVec3;
}

static glm::vec2 to_vector2 ( aiVector3D const & assimpVector3D )
{
    glm::vec2 glmVec2 {};

    glmVec2.x = assimpVector3D.x;
    glmVec2.y = assimpVector3D.y;

    return glmVec2;
}