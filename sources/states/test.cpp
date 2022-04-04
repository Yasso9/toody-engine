#include "test.hpp"

TestState::TestState( std::shared_ptr< sf::RenderWindow > window,
                      Ressources const & ressources )
  : State( window, ressources, State::E_List::Test )
{
    gl::initialize();

    ourShader.loadFromFile( tools::get_path::shaders() + "/shader.vert"s,
                            tools::get_path::shaders() + "/shader.frag"s );
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
    unsigned int VBO, EBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( vertices ),
                  vertices,
                  GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  sizeof( indices ),
                  indices,
                  GL_STATIC_DRAW );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

    // position attribute
    glVertexAttribPointer( 0,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           8 * sizeof( float ),
                           ( void * )0 );
    glEnableVertexAttribArray( 0 );
    // color attribute
    glVertexAttribPointer( 1,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           8 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );
    // texture coord attribute
    glVertexAttribPointer( 2,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           8 * sizeof( float ),
                           ( void * )( 6 * sizeof( float ) ) );
    glEnableVertexAttribArray( 2 );

#pragma GCC diagnostic pop

    bool textureLoad { true };
    // TYPO vérifié que cette operateur marche bien
    textureLoad &=
        texture.loadFromFile( tools::get_path::resources() + "/wall.jpg"s );
    textureLoad &= texture.generateMipmap();
    if ( ! textureLoad )
    {
        throw std::runtime_error { "Cannot load texture"s };
    }
}

void TestState::update() {}

void TestState::render()
{
    // render
    // ------
    gl::clear_window();

    // bind Texture
    // glBindTexture( GL_TEXTURE_2D, texture );
    sf::Texture::bind( &texture );

    // render container
    sf::Shader::bind( &ourShader );

    glBindVertexArray( VAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

    sf::Texture::bind( NULL );
    sf::Shader::bind( NULL );
}