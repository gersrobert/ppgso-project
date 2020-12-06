#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include <cmake-build-debug/shaders/water_vert_glsl.h>
#include <cmake-build-debug/shaders/water_tcs_glsl.h>
#include <cmake-build-debug/shaders/water_tes_glsl.h>
#include <cmake-build-debug/shaders/water_gs_glsl.h>
#include <src/scene/scenes/game_scene.h>
#include "water.h"

std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> Water::shader;

float linearInterpolate(float x1, float x2, float t) {
    return x1 + t * (x2 - x1);
}

Water::Water(Scene &scene, Chunk &chunk) : VertexObject(), chunk(chunk) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = water_vert_glsl;
        shaderConfig.tcs = water_tcs_glsl;
        shaderConfig.tes = water_tes_glsl;
        shaderConfig.gs = water_gs_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_1.bmp"));

    uint32_t samples = 16;
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
    setVertexBuffer(shader);
    setTextureBuffer(shader);
    setNormalBuffer(shader);
    setIndexBuffer();
}

bool Water::update(Scene &scene, float dt) {
    auto gameScene = dynamic_cast<GameScene*>(&scene);

    time += dt;

    textureOffset.x += gameScene->windDirection.x * 0.0125f * dt;
    textureOffset.y += gameScene->windDirection.y * 0.0125f * dt;

    generateModelMatrix();
    return chunk.isActive;
}

void Water::render(Scene &scene) {
    shader->use();

    auto gameScene = dynamic_cast<GameScene*>(&scene);

    // Set up light
    shader->setUniform("LightDirection", gameScene->lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", gameScene->camera->projectionMatrix);
    shader->setUniform("ViewMatrix", gameScene->camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("Transparency", 0.75f);
    shader->setUniform("TextureOffset", textureOffset);
    shader->setUniform("CameraPosition", gameScene->camera->getTotalPosition());
    shader->setUniform("viewDistance", gameScene->VISIBILITY);
    shader->setUniform("Time", time);
    shader->setUniform("BoatPosition", gameScene->targetPosition);
    shader->setUniform("specularFocus", 4.0f);
    shader->setUniform("specularIntensity", 0.8f);
    shader->setUniform("ambientIntensity", 0.9f);
    shader->setUniform("diffuseIntensity", 0.8f);

    glBindVertexArray(vao);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glDrawElements(GL_PATCHES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}