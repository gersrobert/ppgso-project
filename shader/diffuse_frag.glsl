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

uniform float ambientIntensity = 0.2;
uniform float specularFocus;
uniform float specularIntensity;
uniform float diffuseIntensity = 1.0;

uniform float viewDistance = 0.0;

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
  float diffuse = max(dot(inData.normal, vec4(LightDirection, 1.0f)), ambientIntensity) * diffuseIntensity;

  vec3 lightDir = LightDirection;
  lightDir.x *= -1;
  lightDir.y *= -1;
  lightDir.z *= -1;

  vec3 reflectedVector = normalize(reflect(lightDir, inData.normal.xyz));
  vec3 worldToEyeVector = normalize(CameraPosition - inData.worldPosition.xyz);
  float specularFactor = dot(worldToEyeVector, reflectedVector);

  if (specularFactor > 0) {
    specularFactor = pow(specularFactor, specularFocus) * specularIntensity;
  } else {
    specularFactor = 0;
  }

  FragmentColor = texture(Texture, vec2(inData.texCoord.x, 1.0 - inData.texCoord.y) + TextureOffset) * diffuse * (1 + specularFactor);
  FragmentColor.a = Transparency;

  if (viewDistance != 0) {
    float fragDistanceFactor = distance(inData.worldPosition.xyz, CameraPosition) / (viewDistance);
    if (fragDistanceFactor > 0.8) {
      fragDistanceFactor = clamp(fragDistanceFactor * 5 - 4, 0, 1);
      FragmentColor = mix(FragmentColor, vec4(.675f, .82f, .82f, 1), fragDistanceFactor);
    }
  }
}
