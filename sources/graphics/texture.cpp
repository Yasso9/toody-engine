#include "texture.hpp"

#include "tools/string.hpp"
#include "tools/tools.hpp"

#include <iostream>

void Texture::load()
{
    // // load and create a texture
    // glGenTextures( 1, &this->m_id );

    // // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // glBindTexture( GL_TEXTURE_2D, this->m_id );

    // // set the texture wrapping parameters
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // // set texture filtering parameters
    // glTexParameteri( GL_TEXTURE_2D,
    //                  GL_TEXTURE_MIN_FILTER,
    //                  GL_LINEAR_MIPMAP_LINEAR );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // // load image, create texture and generate mipmaps
    // std::string const imagePath { tools::get_path::resources() + "/wall.jpg"s };
    // int width, height, nrChannels;
    // unsigned char * data {
    //     stbi_load( imagePath.c_str(), &width, &height, &nrChannels, 0 )
    // };
    // if ( ! data )
    // {
    //     throw std::runtime_error { "Failed to load texture"s };
    // }

    // glTexImage2D( GL_TEXTURE_2D,
    //               0,
    //               GL_RGB,
    //               width,
    //               height,
    //               0,
    //               GL_RGB,
    //               GL_UNSIGNED_BYTE,
    //               data );
    // glGenerateMipmap( GL_TEXTURE_2D );

    // stbi_image_free( data );
}

void Texture::bind() const
{
    // glBindTexture( GL_TEXTURE_2D, this->m_id );
}