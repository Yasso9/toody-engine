#include "texture.hpp"

#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
#pragma GCC diagnostic ignored "-Wunused-function"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS
#include <OTHERS/stb_image.h>

#pragma GCC diagnostic pop

#include "tools/string.hpp"

unsigned int TextureFromFile( const char * path, const std::string & directory )
{
    std::string filename = std::string { path };
    filename             = directory + filename;

    unsigned int textureID;
    glGenTextures( 1, &textureID );

    int width, height, nrComponents;
    std::cout << "filename : " << filename << std::endl;
    stbi_set_flip_vertically_on_load( true );
    unsigned char * data =
        stbi_load( filename.c_str(), &width, &height, &nrComponents, 3 );
    if ( data != NULL )
    {
        GLenum format;
        if ( nrComponents == 1 )
            format = GL_RED;
        else if ( nrComponents == 3 )
            format = GL_RGB;
        else if ( nrComponents == 4 )
            format = GL_RGBA;
        else
        {
            throw std::runtime_error { ""s };
        }

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
    }
    else
    {
        stbi_image_free( data );
        throw std::runtime_error { "ERROR : Texture failed to load at path: " };
    }

    return textureID;
}

// #include "tools/string.hpp"
// #include "tools/tools.hpp"

// #include <iostream>

// void Texture::load()
// {
//     // // load and create a texture
//     // glGenTextures( 1, &this->m_id );

//     // // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
//     // glBindTexture( GL_TEXTURE_2D, this->m_id );

//     // // set the texture wrapping parameters
//     // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//     // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//     // // set texture filtering parameters
//     // glTexParameteri( GL_TEXTURE_2D,
//     //                  GL_TEXTURE_MIN_FILTER,
//     //                  GL_LINEAR_MIPMAP_LINEAR );
//     // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

//     // // load image, create texture and generate mipmaps
//     // std::string const imagePath { tools::get_path::resources() + "/wall.jpg"s };
//     // int width, height, nrChannels;
//     // unsigned char * data {
//     //     stbi_load( imagePath.c_str(), &width, &height, &nrChannels, 0 )
//     // };
//     // if ( ! data )
//     // {
//     //     throw std::runtime_error { "Failed to load texture"s };
//     // }

//     // glTexImage2D( GL_TEXTURE_2D,
//     //               0,
//     //               GL_RGB,
//     //               width,
//     //               height,
//     //               0,
//     //               GL_RGB,
//     //               GL_UNSIGNED_BYTE,
//     //               data );
//     // glGenerateMipmap( GL_TEXTURE_2D );

//     // stbi_image_free( data );
// }

// void Texture::bind() const
// {
//     // glBindTexture( GL_TEXTURE_2D, this->m_id );
// }