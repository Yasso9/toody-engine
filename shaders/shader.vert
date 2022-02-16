#version 330 core

// the position variable has attribute position
layout (location = 0) in vec3 aPos;
// the color variable has attribute position 1s
layout (location = 1) in vec3 aColor;

out vec3 vertexColor; // specify a color output to the fragment shader

void main()
{
    // see how we directly give a vec3 to vec4's constructor
    gl_Position = vec4(aPos, 1.0);
    // set ourColor to the input color we got from the vertex data
    vertexColor = aColor;
}