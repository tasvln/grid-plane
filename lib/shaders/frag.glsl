#version 330 core

in vec3 worldPos;
out vec4 FragColor;

uniform float spacing = 1.0;
uniform float line = 0.03;
uniform vec3 gridColor = vec3(0.2, 0.6, 1.0);
uniform vec3 bgColor = vec3(0.0, 0.0, 0.0);

void main()
{
  // Compute distance to the nearest grid line in X and Z directions
  float lineX = abs(fract(worldPos.x / spacing - 0.5) - 0.5);
  float lineZ = abs(fract(worldPos.z / spacing - 0.5) - 0.5);

  // Blend line color based on the line Thickness
  float blend = 1.0 - min(min(lineX, lineZ) / line, 1.0);
  vec3 color = mix(bgColor, gridColor, blend);

  FragColor = vec4(color, 1.0);
}
