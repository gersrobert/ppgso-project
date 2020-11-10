//
// Created by Robert Gers on 09/11/2020.
//

#include "rudder.h"
#include "boat.h"
#include "src/scene/scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Rudder::mesh;
std::unique_ptr<ppgso::Texture> Rudder::texture;
std::unique_ptr<ppgso::Shader> Rudder::shader;

Rudder::Rudder(Boat &_boat) : boat(_boat) {
    // Scale the default model
    scale *= 1.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("rudder.obj");

    position.y -= 0.4f;
    rotation.y =  -0.15f;
}

bool Rudder::update(Scene &scene, float dt) {
    position = boat.position;
    rotation = boat.rotation;

    generateModelMatrix();

    steeringLeft = scene.keyboard[GLFW_KEY_D];
    steeringRight = scene.keyboard[GLFW_KEY_A];

    return true;
}

void Rudder::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
