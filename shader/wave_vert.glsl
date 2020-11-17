#version 440
// The inputs will be fed by the vertex buffer objects
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform float Time;
uniform vec3 BoatPosition;

// This will be passed to the fragment shader
out vec2 texCoord;

// Normal to pass to the fragment shader
out vec4 normal;

void main() {
  vec3 pos = Position;
  pos.y += 0.1f * sin(0.5f * Position.x + 0.5f * Position.z + 1 * Time);

  if (Position.z - BoatPosition.z < 0) {
    float deltaX = Position.x - BoatPosition.x;
    if (deltaX < 3f && deltaX > -3f) {
      float y = cos(1.5f * deltaX - 0.5f);
      if (y > 0) {
        pos.y -= 0.2f * y;
      }
    }
  }


  // Copy the input to the fragment shader
  texCoord = TexCoord;

  // Normal in world coordinates
  normal = normalize(ModelMatrix * vec4(Normal, 0.0f));

  // Calculate the final position on screen
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0);
}
