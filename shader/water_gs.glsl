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
    vec3 posses[3];
    for (int i = 0; i < 3; i++) {
        posses[i] = gl_in[i].gl_Position.xyz;
        posses[i].y += 0.1f * sin(0.5f * inData[i].worldPosition.x + 0.5f * inData[i].worldPosition.z + 1 * Time);
    }

    vec3 normal = normalize(cross(posses[1] - posses[0], posses[2] - posses[0]));
    normal.y *= -1;

    for (int i = 0; i < 3; i++) {
        outData.texCoord = inData[i].texCoord;
        outData.worldPosition = inData[i].worldPosition;
        outData.normal = vec4(normal, inData[i].normal.w);

//        if (inData[i].worldPosition.z - BoatPosition.z < 0) {
//            float deltaX = inData[i].worldPosition.x - BoatPosition.x;
//            if (deltaX < 3f && deltaX > -3f) {
//                float y = cos(1.5f * deltaX - 0.5f);
//                if (y > 0) {
//                    posses[i].y -= 0.2f * y;
//                }
//            }
//        }

        gl_Position = vec4(posses[i], gl_in[i].gl_Position.w);
        EmitVertex();
    }
}
