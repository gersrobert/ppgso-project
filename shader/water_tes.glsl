#version 440

layout(triangles) in;

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

void main() {
    outData.texCoord = inData[0].texCoord;
    outData.worldPosition = inData[0].worldPosition;
    outData.normal = inData[0].normal;

    outData.texCoord =  (gl_TessCoord.x * inData[0].texCoord) +
                        (gl_TessCoord.y * inData[1].texCoord) +
                        (gl_TessCoord.z * inData[2].texCoord);

    outData.worldPosition =  (gl_TessCoord.x * inData[0].worldPosition) +
                        (gl_TessCoord.y * inData[1].worldPosition) +
                        (gl_TessCoord.z * inData[2].worldPosition);

    outData.normal =  (gl_TessCoord.x * inData[0].normal) +
                        (gl_TessCoord.y * inData[1].normal) +
                        (gl_TessCoord.z * inData[2].normal);

    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +
                  (gl_TessCoord.y * gl_in[1].gl_Position) +
                  (gl_TessCoord.z * gl_in[2].gl_Position);
}
