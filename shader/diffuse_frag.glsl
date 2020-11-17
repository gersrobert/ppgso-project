#version 440
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform vec3 CameraPosition;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;
in vec4 worldPosition;

in vec4 gl_FragCoord;

// The final color
out vec4 FragmentColor;

void main() {
  // Compute diffuse lighting
  float diffuse = max(dot(normal, vec4(LightDirection, 1.0f)), 0.2f);

  vec3 reflectedVector = normalize(reflect(LightDirection, normal.xyz));
  vec3 worldToEyeVector = normalize(CameraPosition - worldPosition.xyz);
  float specularFactor = dot(worldToEyeVector, reflectedVector);

  if (specularFactor > 0) {
    specularFactor = pow(specularFactor, 64);
  } else {
    specularFactor = 0;
  }

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * diffuse * (1 + specularFactor);
  FragmentColor.a = Transparency;
}
