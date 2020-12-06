#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <src/scene/scenes/game_scene.h>
#include "tree.h"

std::unique_ptr<ppgso::Mesh> Tree::mesh;
std::unique_ptr<ppgso::Texture> Tree::texture;
std::unique_ptr<ppgso::Shader> Tree::shader;

Tree::Tree(Scene &scene, Chunk &chunk, glm::vec3 &_position) : chunk(chunk) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("tree_1.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("tree_1.obj");

    position = _position;
    rotation.z = glm::linearRand(0.0f, 1.0f);
    rotation.x = glm::linearRand(-0.15f, 0.15f);
    rotation.y = glm::linearRand(-0.15f, 0.15f);
    scale *= glm::linearRand(25, 50);
}

bool Tree::update(Scene &scene, float dt) {
    generateModelMatrix();
    return chunk.isActive;
}

void Tree::render(Scene &scene) {
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
    shader->setUniform("Transparency", 1.0f);
    shader->setUniform("CameraPosition", gameScene->camera->getTotalPosition());
    shader->setUniform("viewDistance", gameScene->VISIBILITY);

    mesh->render();
}