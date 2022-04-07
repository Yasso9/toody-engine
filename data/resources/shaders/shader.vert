#version 460 core

// position of each point of the model
layout (location = 0) in vec3 a_position;
// Normal
// layout (location = 1) in vec3 a_normal;
// position of each texture coordinate of the model
layout (location = 1) in vec2 a_textureCoodinate;

// space matrix
uniform mat4 projection;
// camera matrix
uniform mat4 view;
// current object matrix
uniform mat4 model;

// to send to the fragment shader
out vec3 color;
out vec2 textureCoodinate;

void main()
{
    textureCoodinate = a_textureCoodinate;

    gl_Position =
        projection
        * view
        * model
        * vec4(a_position, 1.0);
}