#version 460 core

// in vec3 colorAdded;
in vec2 textureCoodinate;

// color assigned to the current point
out vec4 fragmentColor;

// used to specify the type of our texture
uniform sampler2D texture_diffuse;

void main()
{
    fragmentColor = texture(texture_diffuse, textureCoodinate);
    // fragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}