#version 440
// The inputs will be fed by the vertex buffer objects
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// This will be passed to the fragment shader
out ShaderData {
  vec2 texCoord;
  vec4 worldPosition;
  vec4 normal;
} outData;

void main() {
  // Copy the input to the fragment shader
  outData.texCoord = TexCoord;

  // Normal in world coordinates
  outData.normal = normalize(ModelMatrix * vec4(Normal, 0.0f));
  outData.worldPosition = ModelMatrix * vec4(Position, 1.0);

  // Calculate the final position on screen
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
