#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include "cloud.h"

std::unique_ptr<ppgso::Mesh> Cloud::mesh;
std::unique_ptr<ppgso::Texture> Cloud::texture;
std::unique_ptr<ppgso::Shader> Cloud::shader;

Cloud::Cloud(Scene &scene) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("cloud_2.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cloud_2.obj");

    scale *= 100;
    position = {glm::linearRand(-100.0f, 100.0f), glm::linearRand(50.0f, 100.0f), glm::linearRand(-100.0f, 100.0f)};
}

bool Cloud::update(Scene &scene, float dt) {
    generateModelMatrix();
    return isActive;
}

void Cloud::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection + glm::vec3{0, -1, 0});

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("Transparency", 0.5f);
    shader->setUniform("CameraPosition", scene.camera->position + scene.camera->offset);

    mesh->render();
}