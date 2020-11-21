#include <image_bmp.h>
#include <cmake-build-debug/shaders/color_frag_glsl.h>
#include <cmake-build-debug/shaders/color_vert_glsl.h>
#include "sun.h"
#include "../../scene/scene.h"

std::unique_ptr<ppgso::Mesh> Sun::mesh;
std::unique_ptr<ppgso::Shader> Sun::shader;

Sun::Sun(Scene &scene) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = color_vert_glsl;
        shaderConfig.fs = color_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

    scale *= 10;
}

bool Sun::update(Scene &scene, float dt) {
    position = scene.targetPosition + (100.0f * scene.lightDirection);

    generateModelMatrix();
    return isActive;
}

void Sun::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("OverallColor", {1, .91, .62});

    mesh->render();
}