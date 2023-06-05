#include "shape.hpp"

#include <numeric>                   // for accumulate
#include <string>                    // for operator+, to_string

#include <SFML/Graphics/Shader.hpp>  // for Shader
#include <fmt/core.h>                // for format
#include <glad/glad.h>               // for GLenum, glDeleteBuffers, GL_ELE...

#include "game/resources.hpp"        // for get_texture, get_shader
#include "graphics3D/openGL.hpp"     // for draw_arrays, draw_elements
#include "graphics3D/shape_builder.hpp"  // for ShapeBuilder

unsigned int Shape::S_Data::get_data_per_point_sum() const
{
    return std::accumulate( this->dataPerPoint.begin(),
                            this->dataPerPoint.end(), 0u );
}

unsigned int Shape::S_Data::get_number_of_element() const
{
    return static_cast< unsigned int >( this->vertices.size() )
           / this->get_data_per_point_sum();
}

std::string Shape::S_Data::get_vertex_shader() const
{
    fmt::print( "Hello, world!\n" );
    return R"(
        #version 330 core

        layout (location = 0) in vec3 aPosition;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec3 aTexCoord;
        layout (location = 3) in vec3 aNormal;

        out vec3 fragmentColor;
        out vec3 fragmentTexCoord;
        out vec3 fragmentNormal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            fragmentColor = color;
            fragmentTexCoord = texCoord;
            fragmentNormal = normal;

            gl_Position = projection * view * model * vec4(aPosition, 1.0);
        }
    )";
}

std::string Shape::S_Data::get_fragment_shader() const
{
    return R"(
        #version 330 core

        in vec3 fragmentColor;
        in vec3 fragmentTexCoord;
        in vec3 fragmentNormal;

        out vec4 FragColor;

        uniform sampler2D texture1;

        void main()
        {
            vec3 lightPos = vec3(1.0, 1.0, 1.0);
            vec3 lightColor = vec3(1.0, 1.0, 1.0);
            float ambientStrength = 0.2;
            float diffuseStrength = 0.5;
            float specularStrength = 0.2;
            float shininess = 64.0;

            vec3 ambient = ambientStrength * lightColor;
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * diffuseStrength * lightColor;
            vec3 viewDir = normalize(-FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = specularStrength * spec * lightColor;

            FragColor = texture(texture1, TexCoord) * vec4(ambient + diffuse + specular, 1.0);
        }
    )";
}

sf::Shader & Shape::S_Data::get_shader() const
{
    static sf::Shader shader {};
    shader.loadFromMemory( this->get_vertex_shader(),
                           this->get_fragment_shader() );
    return shader;
}

Shape::Shape( Camera const & camera, S_Data const & data )
  : Transformable { camera, data.get_shader() },
    m_textures {},
    m_vertexArrayObject {},
    m_vertexBufferObject {},
    m_elementBufferObject {},
    m_data { data }
{
    ShapeBuilder { *this }.construct();
}

Shape::~Shape()
{
    ShapeBuilder { *this }.destroy();
}

void Shape::update( UpdateContext & /* context */ )
{
    for ( unsigned int i = 0u; i < m_textures.size(); ++i )
    {
        this->get_shader().setUniform( "my_texture" + std::to_string( i ),
                                       m_textures[i] );
    }
}

void Shape::render( RenderContext & /* context */ ) const
{
    GLenum const primitiveType { GL_TRIANGLES };

    if ( this->is_EBO_handled() )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject );
        GLenum const dataType { GL_UNSIGNED_INT };
        gl::draw_elements( m_vertexArrayObject, primitiveType, dataType,
                           m_data.vertices.size() );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0u );
    }
    else
    {
        gl::draw_arrays( m_vertexArrayObject, primitiveType,
                         m_data.get_number_of_element() );
    }
}

unsigned int Shape::get_VAO() const
{
    return const_cast< Shape * >( this )->get_VAO();
}

unsigned int & Shape::get_VAO()
{
    return m_vertexArrayObject;
}

unsigned int Shape::get_VBO() const
{
    return const_cast< Shape * >( this )->get_VBO();
}

unsigned int & Shape::get_VBO()
{
    return m_vertexBufferObject;
}

unsigned int Shape::get_EBO() const
{
    return const_cast< Shape * >( this )->get_EBO();
}

unsigned int & Shape::get_EBO()
{
    return m_elementBufferObject;
}

Shape::S_Data const & Shape::get_data() const
{
    return m_data;
}

bool Shape::is_EBO_handled() const
{
    return this->m_data.indices.has_value();
}

void Shape::set_texture( sf::Texture const & texture )
{
    m_textures.push_back( texture );
}
