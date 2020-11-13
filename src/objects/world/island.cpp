#include <src/util/bezier.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include "island.h"
#include "src/scene/scene.h"

std::unique_ptr<ppgso::Mesh> Island::mesh;
std::unique_ptr<ppgso::Texture> Island::texture;
std::unique_ptr<ppgso::Shader> Island::shader;

Island::Island(Scene &scene, Chunk &chunk) : chunk(chunk) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("island_1.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("island_1.obj");

    meshIndex = glm::linearRand((uint32_t) 0, (uint32_t) 2);
    textureIndex = glm::linearRand((uint32_t) 0, (uint32_t) 2);

    position = {
            glm::linearRand(
                    -chunk.size + chunk.size * chunk.position.x * 2,
                    chunk.size + chunk.size * chunk.position.x * 2),
            -0.1f,
            glm::linearRand(
                    -chunk.size + chunk.size * chunk.position.y * 2,
                    chunk.size + chunk.size * chunk.position.y * 2)
    };
    scale *= 10;
    rotation.z = glm::linearRand(0.0f, ppgso::PI * 2);
}

bool Island::update(Scene &scene, float dt) {
    generateModelMatrix();
    return chunk.isActive;
}

void Island::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("Transparency", 1.0f);
    shader->setUniform("CameraPosition", scene.camera->position + scene.camera->offset);

    mesh->render();
}