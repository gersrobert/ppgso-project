#version 440
// The inputs will be fed by the vertex buffer objects
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform float sailRotation;

// This will be passed to the fragment shader
out ShaderData {
  vec2 texCoord;
  vec4 worldPosition;
  vec4 normal;
} outData;

void main() {
  vec3 pos = Position;
  if (Position.z < 0.5 && Position.y < 0.5) {
    pos.x -= 6 * sin(sailRotation);
    pos.y += abs(1 * sin(sailRotation));
    pos.z -= 3 * cos(sailRotation) - 3;
  }

  // Copy the input to the fragment shader
  outData.texCoord = TexCoord;

  // Normal in world coordinates
  outData.normal = normalize(ModelMatrix * vec4(Normal, 0.0f));
  outData.worldPosition = ModelMatrix * vec4(pos, 1.0);

  // Calculate the final position on screen
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0);
}
