#version 330 core

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

uniform vec2 screenResolution;

void main()
{
    vec4 linearColor = texture(screenTexture, texCoords);

    // Convert from sRGB to linear space
    vec3 color = linearColor.rgb / linearColor.a;

    // Apply gamma correction
    color = pow(color, vec3(2.2));

    // Convert back to premultiplied alpha
    linearColor.rgb = color * linearColor.a;

    // Output the final color
    fragColor = vec4(1.0) - linearColor;
}