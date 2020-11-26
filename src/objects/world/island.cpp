#include <src/util/bezier.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <cmake-build-debug/shaders/volumetric_cloud_frag_glsl.h>
#include "island.h"
#include "src/scene/scene.h"
#include "tree.h"

std::unique_ptr<ppgso::Mesh> Island::mesh;
std::unique_ptr<ppgso::Texture> Island::texture;
std::unique_ptr<ppgso::Shader> Island::shader;

Island::Island(Scene &scene, Chunk &chunk) : chunk(chunk) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("island_1.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("island_1.obj");

    meshIndex = glm::linearRand((uint32_t) 0, (uint32_t) 2);
    textureIndex = glm::linearRand((uint32_t) 0, (uint32_t) 2);

    position = {
            glm::linearRand(
                    -chunk.size + chunk.size * chunk.position.x * 1.8f,
                    chunk.size + chunk.size * chunk.position.x * 1.8f),
            -1.0f,
            glm::linearRand(
                    -chunk.size + chunk.size * chunk.position.y * 1.8f,
                    chunk.size + chunk.size * chunk.position.y * 1.8f)
    };
    scale *= 100;
    rotation.z = glm::linearRand(0.0f, ppgso::PI * 2);

    for (size_t i = 0; i < glm::linearRand(0, 3); ++i) {
        glm::vec3 pos = position + glm::vec3{
            glm::linearRand(-5.0f + float(i) * 5.0f, -5.0f + float(i+1) * 5.0f),
            2,
            glm::linearRand(-5.0f + float(i) * 5.0f, -5.0f + float(i+1) * 5.0f),
        };

        auto tree = std::make_unique<Tree>(scene, chunk, pos);
        scene.objects.push_back(move(tree));
    }
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
    shader->setUniform("CameraPosition", scene.camera->getTotalPosition());
    shader->setUniform("viewDistance", scene.VISIBILITY);

    mesh->render();
}