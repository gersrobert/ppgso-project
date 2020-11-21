#version 440

layout(triangles) in;

layout (triangle_strip, max_vertices=3) out;

in ShaderData {
    vec2 texCoord;
    vec4 worldPosition;
    vec4 normal;
} inData[];

out ShaderData {
    vec2 texCoord;
    vec4 worldPosition;
    vec4 normal;
} outData;

uniform float Time;
uniform vec3 BoatPosition;

void main() {
    for (int i = 0; i < 3; i++) {
        outData.texCoord = inData[i].texCoord;
        outData.worldPosition = inData[i].worldPosition;
        outData.normal = inData[i].normal;

        vec4 pos = gl_in[i].gl_Position;

        pos.y += 0.1f * sin(0.5f * inData[i].worldPosition.x + 0.5f * inData[i].worldPosition.z + 1 * Time);
        if (inData[i].worldPosition.z - BoatPosition.z < 0) {
            float deltaX = inData[i].worldPosition.x - BoatPosition.x;
            if (deltaX < 3f && deltaX > -3f) {
                float y = cos(1.5f * deltaX - 0.5f);
                if (y > 0) {
                    pos.y -= 0.2f * y;
                }
            }
        }

        gl_Position = pos;
        EmitVertex();
    }
}
