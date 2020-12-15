#include <src/scene/scene_window.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "static_object.h"

StaticObject::StaticObject(Scene &scene, const std::string &meshName, const std::string &textureName) {
    ppgso::ShaderConfig shaderConfig;
    shaderConfig.vs = diffuse_vert_glsl;
    shaderConfig.fs = diffuse_frag_glsl;
    shader = std::make_unique<ppgso::Shader>(shaderConfig);

    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureName));
    mesh = std::make_unique<ppgso::Mesh>(meshName);
}

bool StaticObject::update(Scene &scene, float dt) {
    generateModelMatrix();
    return isActive;
}

void StaticObject::render(Scene &scene) {
    shader->use();

    auto gameScene = dynamic_cast<GameScene *>(&scene);

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
