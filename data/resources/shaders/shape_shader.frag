#version 460 core

// in vec3 colorAdded;
in vec2 textureCoodinate;

// color assigned to the current point
out vec4 fragmentColor;

// used to specify the type of our texture
uniform sampler2D my_textureA;
uniform sampler2D my_textureB;

void main()
{
    vec4 textureA = texture(my_textureA, textureCoodinate);
    vec4 textureB = texture(my_textureB, textureCoodinate);
    if(textureB.a < 0.1)
        discard;

    fragmentColor = mix(textureA, textureB, 0.2);
    // fragmentColor = textureA * 0.8 +
    //                 textureB * 0.2;
    // fragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}