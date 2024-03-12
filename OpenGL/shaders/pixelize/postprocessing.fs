#version 330 core

out vec4 fragColor;
in vec2 texCoords;


uniform sampler2D colorTexture;
uniform sampler2D positionTexture;
uniform sampler2D screenTexture;

uniform vec2 screenResolution;
uniform int pixelSize;

void main()
{
    
  int pixelSize = pixelSize;

  vec2 texSize  = textureSize(colorTexture, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  vec4 position = texture(positionTexture, texCoord);

  if (position.a <= 0.0) {
    fragColor = texture(colorTexture, texCoord);

    return;
  }

  float x = int(gl_FragCoord.x) % pixelSize;
  float y = int(gl_FragCoord.y) % pixelSize;

        x = floor(pixelSize / 2.0) - x;
        y = floor(pixelSize / 2.0) - y;

        x = gl_FragCoord.x + x;
        y = gl_FragCoord.y + y;

  vec2 uv = vec2(x, y) / texSize;

  fragColor = texture(colorTexture, uv);
}