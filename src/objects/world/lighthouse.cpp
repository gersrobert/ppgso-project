#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "lighthouse.h"

std::unique_ptr<ppgso::Shader> LightHouse::shader;
std::unique_ptr<ppgso::Mesh> LightHouse::mesh;
std::unique_ptr<ppgso::Texture> LightHouse::texture;

LightHouse::LightHouse(Scene &scene) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("lighthouse.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("lighthouse.obj");
}

bool LightHouse::update(Scene &scene, float dt) {
    generateModelMatrix();
    return isActive;
}

void LightHouse::render(Scene &scene) {
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