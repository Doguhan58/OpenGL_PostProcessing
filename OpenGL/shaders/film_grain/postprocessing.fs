#version 330 core

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform vec2 screenResolution;
uniform float time;
uniform float strength;

void main()
{
    vec2 uv = gl_FragCoord.xy/ screenResolution.xy;
    float mdf = strength; 
    float noise = (fract(sin(dot(uv, vec2(12.98,68.23)*2.0)) * 44668.5253));
    vec4 tex = texture(screenTexture, uv);

    mdf *= sin(time) + 1.0;
    vec4 col = tex - noise * strength;
    fragColor = col;
}
