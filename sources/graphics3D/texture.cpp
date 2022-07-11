#include "texture.hpp"

#include <iostream>

Texture::Texture( std::string const & filePath, Texture::E_Type const & type )
  : m_type( type ), m_path( filePath )
{
    if ( ! this->loadFromFile( filePath ) )
    {
        throw std::runtime_error { "Loading Texture '"s + filePath
                                   + "' not successfull"s };
    }
    if ( ! this->generateMipmap() )
    {
        throw std::runtime_error { "Cannot generate MipMap for texture '"s
                                   + filePath + '\'' };
    }

    std::cout << "Texture Loaded : " << filePath << std::endl;
}

Texture::E_Type Texture::get_type() const
{
    return this->m_type;
}

std::string Texture::get_path() const
{
    return this->m_path;
}

std::string Texture::get_type_name() const
{
    switch ( this->m_type )
    {
    case Texture::E_Type::Diffuse :
        return "texture_diffuse"s;
    case Texture::E_Type::Specular :
        return "texture_diffuse"s;
    case Texture::E_Type::Normal :
        return "texture_diffuse"s;
    case Texture::E_Type::Height :
        return "texture_diffuse"s;
    default :
        throw std::runtime_error { "Type not supported"s };
        break;
    }
}

aiTextureType Texture::to_assimp_type( Texture::E_Type const & type )
{
    switch ( type )
    {
    case Texture::E_Type::Diffuse :
        return aiTextureType::aiTextureType_DIFFUSE;
    case Texture::E_Type::Specular :
        return aiTextureType::aiTextureType_SPECULAR;
    case Texture::E_Type::Normal :
        return aiTextureType::aiTextureType_NORMALS;
    case Texture::E_Type::Height :
        return aiTextureType::aiTextureType_HEIGHT;
    default :
        throw std::runtime_error { "Type not supported"s };
        break;
    }
}

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS
#include <OTHERS/stb_image.h>

#include "graphics3D/openGL.hpp"

namespace GLTexture
{
    unsigned int load( std::string const & filePath )
    {
        unsigned int textureID;
        glGenTextures( 1, &textureID );

        std::cout << "Texture loaded : " << filePath << std::endl;

        stbi_set_flip_vertically_on_load( true );

        int width, height, nrComponents;
        unsigned char * data {
            stbi_load( filePath.c_str(), &width, &height, &nrComponents, 3 )
        };
        if ( data == NULL )
        {
            stbi_image_free( data );
            throw std::runtime_error {
                "ERROR : Texture failed to load at path: '"s + filePath + "'"s
            };
        }

        GLenum format;
        switch ( nrComponents )
        {
        case 1 :
            format = GL_RED;
            break;
        case 3 :
            format = GL_RGB;
            break;
        case 4 :
            format = GL_RGBA;
            break;
        default :
            throw std::runtime_error { "Error while loading the texture"s };
            break;
        }

        // So we can have image with width and height that are not the same
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
        glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
        glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );

        // std::cout << "Texture ID : " << textureID << std::endl;
        glBindTexture( GL_TEXTURE_2D, textureID );
        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      static_cast< GLint >( format ),
                      width,
                      height,
                      0,
                      format,
                      GL_UNSIGNED_BYTE,
                      data );
        glGenerateMipmap( GL_TEXTURE_2D );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D,
                         GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        stbi_image_free( data );

        return textureID;
    }
} // namespace GLTexture