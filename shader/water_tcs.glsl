#version 440

layout(vertices=3) out;

in ShaderData {
    vec2 texCoord;
    vec4 worldPosition;
    vec4 normal;
} inData[];

out ShaderData {
    vec2 texCoord;
    vec4 worldPosition;
    vec4 normal;
} outData[];

uniform vec3 CameraPosition;
uniform vec3 BoatPosition;
uniform float viewDistance;

void main() {
    outData[gl_InvocationID].texCoord = inData[gl_InvocationID].texCoord;
    outData[gl_InvocationID].worldPosition = inData[gl_InvocationID].worldPosition;
    outData[gl_InvocationID].normal = inData[gl_InvocationID].normal;

    float dist = distance(BoatPosition.xz, inData[gl_InvocationID].worldPosition.xz);
    float tessFactor = 1;
    if (dist < 0.3 * viewDistance) {
        tessFactor = 10000;
    } else if (dist< 1.2 * viewDistance) {
        tessFactor = 25;
    }

    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = tessFactor;
        gl_TessLevelOuter[0] = tessFactor;
        gl_TessLevelOuter[1] = tessFactor;
        gl_TessLevelOuter[2] = tessFactor;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
