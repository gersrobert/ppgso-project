#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <src/scene/scenes/game_scene.h>
#include "tree.h"
#include "island.h"
#include "tree_bird.h"

std::unique_ptr<ppgso::Mesh> Tree::mesh;
std::unique_ptr<ppgso::Texture> Tree::texture;
std::unique_ptr<ppgso::Shader> Tree::shader;

Tree::Tree(Scene &scene, Chunk &chunk, Island &island) : chunk(chunk), island(island) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("tree_1.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("tree_1.obj");

    position = {
            glm::linearRand(-0.05f, 0.05f),
            0,
            glm::linearRand(-0.05f, 0.05f),
    };

    rotation.y = glm::linearRand(0.0f, 1.0f);
    rotation.z = glm::linearRand(-0.1f, 0.1f);
    rotation.x = glm::linearRand(-0.1f, 0.1f);
    scale *= glm::linearRand(0.5, 1.0);

    if (glm::linearRand(1, 1)) {
        auto bird = std::make_unique<TreeBird>(scene, chunk, *this);
        scene.objects.push_back(move(bird));
    }
}

bool Tree::update(Scene &scene, float dt) {
    modelMatrix = island.modelMatrix
            * glm::translate(glm::mat4(1), position)
            * glm::rotate(glm::mat4(1), rotation.x, {1, 0, 0})
            * glm::rotate(glm::mat4(1), rotation.y, {0, 1, 0})
            * glm::rotate(glm::mat4(1), rotation.z, {0, 0, 1})
            * glm::scale(glm::mat4(1), scale);

    return chunk.isActive;
}

void Tree::render(Scene &scene) {
    shader->use();

    auto gameScene = dynamic_cast<GameScene*>(&scene);

    // Set up light
    shader->setUniform("LightDirection", gameScene->lightDirection);
    shader->setUniform("lightColor", gameScene->lightColor);

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