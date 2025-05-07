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
  float scale = 2.0; // Increase to make lines visually thicker
  vec2 grid = abs(fract(worldPos.xz / spacing * scale) - 0.5);

  // Calculate minimum distance to a grid line
  float line = min(grid.x, grid.y);

  float thickness = 0.002; // desired thickness
  float aa = fwidth(line) * 1.5; // anti-alias amount

  float gridFade = smoothstep(thickness + aa, thickness - aa, line);

  vec3 gridColor = vec3(0.1); // squares
  vec3 backgroundColor = vec3(0.5); // lines

  vec3 finalColor = mix(gridColor, backgroundColor, gridFade);

  FragColor = vec4(finalColor, 1.0);
}
