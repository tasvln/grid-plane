#version 330 core

// Receive color from vertex shader
in vec3 vertexColor;
out vec4 fragColor;

void main()
{
  // Output the interpolated color
  fragColor = vec4(vertexColor, 1.0);
}
