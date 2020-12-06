#version 440
in vec4 vertexColor;

// The final color
out vec4 FragmentColor;

// Additional overall color when not using per-vertex Color input
uniform vec4 OverallColor;

void main() {
  // Just pass the color to the output
  FragmentColor = vertexColor + OverallColor;
}
