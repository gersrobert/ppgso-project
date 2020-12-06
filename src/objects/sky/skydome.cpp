#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <src/scene/scenes/game_scene.h>
#include "skydome.h"

std::unique_ptr<ppgso::Mesh> Skydome::mesh;
std::unique_ptr<ppgso::Texture> Skydome::texture;
std::unique_ptr<ppgso::Shader> Skydome::shader;

Skydome::Skydome(GameScene &scene) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sky_clear.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("skydome.obj");

    scale *= scene.VISIBILITY - 15;
    position.y -= 1.15f;
    rotation.z = 1.4f;
}

bool Skydome::update(Scene &scene, float dt) {
    auto gameScene = dynamic_cast<GameScene*>(&scene);

    position.x = gameScene->targetPosition.x;
    position.z = gameScene->targetPosition.z;

    generateModelMatrix();
    return isActive;
}

void Skydome::render(Scene &scene) {
    shader->use();

    auto gameScene = dynamic_cast<GameScene*>(&scene);

    // Set up light
    shader->setUniform("LightDirection", {0, 0, 0});

    // use camera
    shader->setUniform("ProjectionMatrix", gameScene->camera->projectionMatrix);
    shader->setUniform("ViewMatrix", gameScene->camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("ambientIntensity", 1.0f);
    shader->setUniform("Transparency", 1.0f);
    shader->setUniform("CameraPosition", gameScene->camera->getTotalPosition());

    mesh->render();
}