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

uniform float specularFocus;
uniform float specularIntensity;

// The vertex shader will feed this input
in ShaderData {
  vec2 texCoord;
  vec4 worldPosition;
  vec4 normal;
} inData;

in vec4 gl_FragCoord;

// The final color
out vec4 FragmentColor;

void main() {
  // Compute diffuse lighting
  float diffuse = max(dot(inData.normal, vec4(LightDirection, 1.0f)), 0.2f);

  vec3 reflectedVector = normalize(reflect(LightDirection, inData.normal.xyz));
  vec3 worldToEyeVector = normalize(CameraPosition - inData.worldPosition.xyz);
  float specularFactor = dot(worldToEyeVector, reflectedVector);

  if (specularFactor > 0) {
    specularFactor = pow(specularFactor, specularFocus) * specularIntensity;
  } else {
    specularFactor = 0;
  }

  FragmentColor = texture(Texture, vec2(inData.texCoord.x, 1.0 - inData.texCoord.y) + TextureOffset) * diffuse * (1 + specularFactor);
  FragmentColor.a = Transparency;

//  FragmentColor = vec4(1, 1, 1, 1);
}
