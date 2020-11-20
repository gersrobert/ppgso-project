#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include <cmake-build-debug/shaders/wave_vert_glsl.h>
#include "water.h"

std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> VertexObject::shader;

float linearInterpolate(float x1, float x2, float t) {
    return x1 + t * (x2 - x1);
}

Water::Water(Scene &scene, Chunk &chunk) : chunk(chunk) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(wave_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_1.bmp"));

    uint32_t samples = 1000;
    for (uint32_t i = 0; i < samples; ++i) {
        for (uint32_t j = 0; j < samples; ++j) {
            glm::vec3 vertex = {
                    linearInterpolate(
                            -chunk.size + chunk.size * chunk.position.x * 2.0f,
                            chunk.size + chunk.size * chunk.position.x * 2.0f,
                            float(j) / float(samples - 1)
                    ),
                    0,
                    linearInterpolate(
                            -chunk.size + chunk.size * chunk.position.y * 2.0f,
                            chunk.size + chunk.size * chunk.position.y * 2.0f,
                            float(i) / float(samples - 1)
                    )
            };
            vertices.push_back(vertex);

            texCoords.emplace_back(
                    linearInterpolate(0.0f, chunk.size / 10, float(j) / float(samples - 1)),
                    linearInterpolate(0.0f, chunk.size / 10, float(i) / float(samples - 1))
            );

            normals.emplace_back(0, 1, 0);

            if (i > 0 && j > 0) {

                faces.push_back({
                                        (i - 1) * samples + (j - 1),
                                        (i) * samples + (j - 1),
                                        (i - 1) * samples + (j),
                                });
                faces.push_back({
                                        (i) * samples + (j),
                                        (i - 1) * samples + (j),
                                        (i) * samples + (j - 1),
                                });
            }
        }
    }

    // Copy data to OpenGL
    glBindVertexArray(vao);
    setVertexBuffer();
    setTextureBuffer();
    setNormalBuffer();
    setIndexBuffer();

}

bool Water::update(Scene &scene, float dt) {
    time += dt;

    textureOffset.x += scene.windDirection.x * 0.0125f * dt;
    textureOffset.y += scene.windDirection.y * 0.0125f * dt;

    generateModelMatrix();
    return chunk.isActive;
}

void Water::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("Transparency", 0.75f);
    shader->setUniform("TextureOffset", textureOffset);
    shader->setUniform("CameraPosition", scene.camera->position);
    shader->setUniform("Time", time);
    shader->setUniform("BoatPosition", scene.targetPosition);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}