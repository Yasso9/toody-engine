#version 460 core

// in vec3 colorAdded;
in vec2 textureCoodinate;

// color assigned to the current point
out vec4 fragmentColor;

// used to specify the type of our texture
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
    vec4 colorDiffuse = texture(texture_diffuse1, textureCoodinate);
    vec4 colorSpecular = texture(texture_specular1, textureCoodinate);
    vec4 colorNormal = texture(texture_normal1, textureCoodinate);

    fragmentColor = colorDiffuse * 0.9 +
                    colorSpecular * 0.05 +
                    colorNormal * 0.05;
    // fragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}