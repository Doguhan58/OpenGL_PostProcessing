#version 330 core

uniform sampler2D colorTexture;

out vec4 fragColor;

uniform int size;

void main() {
  vec2 texSize  = textureSize(colorTexture, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  fragColor = texture(colorTexture, texCoord);

  int size = size;
  if (size <= 0) { return; }

  float separation = 1;
        separation = max(separation, 1);

  fragColor.rgb = vec3(0);

  float count = 0.0;

  for (int i = -size; i <= size; ++i) {
    for (int j = -size; j <= size; ++j) {
      fragColor.rgb +=
        texture
          ( colorTexture
          ,   ( gl_FragCoord.xy
              + (vec2(i, j) * separation)
              )
            / texSize
          ).rgb;

      count += 1.0;
    }
  }

  fragColor.rgb /= count;
}