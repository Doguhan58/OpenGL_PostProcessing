#version 330 core

uniform sampler2D colorTexture;

out vec4 fragColor;
uniform float amount;

void main() {
  float amount = amount;

  vec2 texSize   = textureSize(colorTexture, 0).xy;
  vec2 fragCoord = gl_FragCoord.xy;
  vec2 texCoord  = fragCoord / texSize;

  float neighbor = amount * -1.0;
  float center   = amount *  4.0 + 1.0;

  vec3 color =
        texture(colorTexture, (fragCoord + vec2( 0,  1)) / texSize).rgb
      * neighbor

      + texture(colorTexture, (fragCoord + vec2(-1,  0)) / texSize).rgb
      * neighbor
      + texture(colorTexture, (fragCoord + vec2( 0,  0)) / texSize).rgb
      * center
      + texture(colorTexture, (fragCoord + vec2( 1,  0)) / texSize).rgb
      * neighbor

      + texture(colorTexture, (fragCoord + vec2( 0, -1)) / texSize).rgb
      * neighbor
      ;

  fragColor = vec4(color, texture(colorTexture, texCoord).a);
}