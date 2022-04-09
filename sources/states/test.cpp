#include "test.hpp"

TestState::TestState() : State( State::E_List::Test )
{
    // configure global opengl state
    // -----------------------------
    glEnable( GL_DEPTH_TEST );

    // build and compile our shader zprogram
    // ------------------------------------
    ourShader.loadFromFile( tools::get_path::shaders() + "/shader.vert"s,
                            tools::get_path::shaders() + "/shader.frag"s );

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
    };
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );

    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( vertices ),
                  vertices,
                  GL_STATIC_DRAW );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
    // position attribute
    glVertexAttribPointer( 0,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           5 * sizeof( float ),
                           ( void * )0 );
    glEnableVertexAttribArray( 0 );
    // texture coord attribute
    glVertexAttribPointer( 1,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           5 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

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

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    // ourShader.use();
    // ourShader.setInt( "texture1", 0 );
    // ourShader.setInt( "texture2", 1 );
}

void TestState::update() {}

void TestState::render() const
{
    // bind Texture
    // glBindTexture( GL_TEXTURE_2D, texture );
    sf::Texture::bind( &texture );

    // render container
    sf::Shader::bind( &ourShader );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"

    // create transformations
    glm::mat4 model = glm::mat4(
        1.0f ); // make sure to initialize matrix to identity matrix first
    glm::mat4 view       = glm::mat4( 1.0f );
    glm::mat4 projection = glm::mat4( 1.0f );
    model      = glm::rotate( model, 4.f, glm::vec3( 0.5f, 1.0f, 0.0f ) );
    view       = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );
    projection = glm::perspective( glm::radians( 45.0f ),
                                   Window::get_instance().get_aspect_ratio(),
                                   0.1f,
                                   100.0f );
    // retrieve the matrix uniform locations
    unsigned int modelLoc =
        glGetUniformLocation( ourShader.getNativeHandle(), "model" );
    unsigned int viewLoc =
        glGetUniformLocation( ourShader.getNativeHandle(), "view" );
    unsigned int projectionLoc =
        glGetUniformLocation( ourShader.getNativeHandle(), "projection" );
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, &view[0][0] );
    glUniformMatrix4fv( projectionLoc,
                        1,
                        GL_FALSE,
                        glm::value_ptr( projection ) );
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    // ourShader.( "projection", projection );

#pragma GCC diagnostic pop

    // render box
    glBindVertexArray( VAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    sf::Texture::bind( NULL );
    sf::Shader::bind( NULL );
}