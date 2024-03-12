#version 330 core

out vec4 fragColor;
in vec2 texCoords;


uniform sampler2D colorTexture;
uniform sampler2D positionTexture;
uniform sampler2D screenTexture;
uniform sampler2D iChannel0;

uniform vec2 screenResolution;
uniform int pixelSize;
uniform float time;



vec4 Posterize(in vec4 inputColor){
  float gamma = 0.3f;
  float numColors = 7.0f;
  

  vec3 color = inputColor.rgb;
  color = pow(color, vec3(gamma, gamma, gamma));
  color = color * numColors;
  color = floor(color);
  color = color / numColors;
  color = pow(color, vec3(1.0/gamma));
  
  return vec4(color, inputColor.a);
}

void main()
{
    
  vec2 uv = gl_FragCoord.xy/screenResolution.xy;
    fragColor = texture(iChannel0, uv);
    // Time varying pixel color
    vec3 col = 0.5 + 0.5*cos(time+uv.xyx+vec3(0,2,4));
    // Output to screen
    fragColor = Posterize(fragColor);
}