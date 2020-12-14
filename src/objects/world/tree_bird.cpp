#include <shaders/bird_vert_glsl.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "tree_bird.h"

std::unique_ptr<ppgso::Shader> TreeBird::shader;
std::unique_ptr<ppgso::Mesh> TreeBird::mesh;
std::unique_ptr<ppgso::Texture> TreeBird::texture;

TreeBird::TreeBird(Scene &scene, Chunk &chunk, Tree &tree) : chunk(chunk), tree(tree) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = bird_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bird.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bird.obj");

    position.y = 0.175f;
    scale *= 0.04f;
    rotation.y = ppgso::PI;
}

bool TreeBird::update(Scene &scene, float dt) {
    time += dt;
    rotation.y += dt * 0.3f;

    modelMatrix = tree.modelMatrix
                  * glm::translate(glm::mat4(1), position)
                    * glm::rotate(glm::mat4(1), rotation.x, {1, 0, 0})
                    * glm::rotate(glm::mat4(1), rotation.y, {0, 1, 0})
                    * glm::rotate(glm::mat4(1), rotation.z, {0, 0, 1})
                  * glm::translate(glm::mat4(1), {-0.2, 0, -0.2})
                  * glm::scale(glm::mat4(1), scale);
    return chunk.isActive;
}

void TreeBird::render(Scene &scene) {
    shader->use();

    auto gameScene = dynamic_cast<GameScene *>(&scene);

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
    shader->setUniform("Time", time);

    mesh->render();
}
