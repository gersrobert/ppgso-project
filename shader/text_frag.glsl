#version 440
// A texture is expected as program attribute
uniform sampler2D Texture;

// (optional) Transparency
uniform float Transparency = 1;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;
in vec4 color;

// The final color
out vec4 FragmentColor;

void main() {
  FragmentColor = texture(Texture, vec2(texCoord.x, texCoord.y) + TextureOffset);
  FragmentColor.a = Transparency;

  if (FragmentColor.r < 0.75 &&FragmentColor.g < 0.75 &&FragmentColor.b < 0.75) {
    FragmentColor.a = 0;
  }
}
