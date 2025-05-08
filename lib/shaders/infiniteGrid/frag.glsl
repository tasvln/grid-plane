#version 330 core

in vec3 worldPos;
out vec4 FragColor;

uniform vec3 cameraPosition;
uniform float spacing;

void main()
{
  // screen-space line thickness
  vec2 dx = vec2(dFdx(worldPos.x), dFdy(worldPos.x));
  vec2 dy = vec2(dFdx(worldPos.z), dFdy(worldPos.z));
  vec2 dudv = vec2(length(dx), length(dy));

  // grid position
  float scale = 2.0;
  vec2 grid = abs(fract(worldPos.xz / spacing * scale));

  float line = min(grid.x, grid.y);
  float thickness = 0.002;
  float aa = fwidth(line) * 1.5;

  // Axis lines (X in red, Z in blue)

  // thickness for Y axis line
  bool isXAxis = abs(worldPos.z) < spacing * thickness;
  // thickness for Z axis line
  bool isZAxis = abs(worldPos.x) < spacing * thickness;

  float dist = length(worldPos.xz - cameraPosition.xz);

  // fade range
  float fade = 1.0 - smoothstep(20.0, 80.0, dist);

  if (isXAxis) {
    FragColor = vec4(vec3(1.0, 0.0, 0.0), fade);
    return;
  }
  if (isZAxis) {
    FragColor = vec4(vec3(0.0, 0.0, 1.0), fade);
    return;
  }

  float gridFade = smoothstep(thickness + aa, thickness - aa, line);

  // grid lines color
  vec3 gridColor = vec3(0.1);
  // between lines color
  vec3 backgroundColor = vec3(0.5);

  vec3 finalColor = mix(gridColor, backgroundColor, gridFade);

  FragColor = vec4(finalColor, fade);
}
