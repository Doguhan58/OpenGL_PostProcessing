#version 330 core

uniform sampler2D colorTexture;

out vec4 fragColor;

const int size = 5;

const int maxSize = ((size * 2 + 1) * (size * 2 + 1));


vec2 texSize  = textureSize(colorTexture, 0).xy;
vec2 texCoord = gl_FragCoord.xy / texSize;

int i     = 0;
int j     = 0;
int count = 0;

vec3  valueRatios = vec3(0.3, 0.59, 0.11);

float values[maxSize];

vec4  color       = vec4(0.0);
vec4  meanTemp    = vec4(0.0);
vec4  mean        = vec4(0.0);
float valueMean   =  0.0;
float variance    =  0.0;
float minVariance = -1.0;

void findMean(int i0, int i1, int j0, int j1) {
  meanTemp = vec4(0);
  count    = 0;

  for (i = i0; i <= i1; ++i) {
    for (j = j0; j <= j1; ++j) {
      color  = texture(colorTexture,(gl_FragCoord.xy + vec2(i, j))/texSize);
      meanTemp += color;
      values[count] = dot(color.rgb, valueRatios);
      count += 1;
    }
  }

  meanTemp.rgb /= count;
  valueMean     = dot(meanTemp.rgb, valueRatios);

  for (i = 0; i < count; ++i) {
    variance += pow(values[i] - valueMean, 2);
  }
  variance /= count;
  if (variance < minVariance || minVariance <= -1) {
    mean = meanTemp;
    minVariance = variance;
  }
}
void main() {
  fragColor = texture(colorTexture, texCoord);

  int size = 5;
  if (size <= 0) { return; }

  // Lower Left
  findMean(-size, 0, -size, 0);
  // Upper Right
  findMean(0, size, 0, size);
  // Upper Left
  findMean(-size, 0, 0, size);
  // Lower Right
  findMean(0, size, -size, 0);
  fragColor.rgb = mean.rgb;
}