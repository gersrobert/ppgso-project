#include "boat.h"
#include "src/scene/scene.h"
#include "rudder.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Boat::mesh;
std::unique_ptr<ppgso::Texture> Boat::texture;
std::unique_ptr<ppgso::Shader> Boat::shader;

Boat::Boat(Scene &scene) {
    // Scale the default model
    scale *= 0.5f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("orange_boat.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("orange_boat.obj");

    position.y -= 1.2f;
    rotation.y =  -0.15f;
    rotation.x = -0.015f;
}

bool Boat::update(Scene &scene, float dt) {
    // Hit detection
    for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;
    }

    // Keyboard controls
    if (rotationSpeed > 0) {
        rotationSpeed -=  0.001f * dt;
    } else {
        rotationSpeed += 0.001f * dt;
    }
    if (scene.keyboard[GLFW_KEY_D]) {
        rotationSpeed -= 0.003f * dt;
    }
    else if (scene.keyboard[GLFW_KEY_A]) {
        rotationSpeed += 0.003f * dt;
    }

    const float maxRotation = 0.005f;
    if (rotationSpeed > maxRotation) {
        rotationSpeed = maxRotation;
    }
    if (rotationSpeed < -maxRotation) {
        rotationSpeed = -maxRotation;
    }

    rotation.z += rotationSpeed;
    position.z += 0.05f * std::cos(rotation.z);
    position.x += 0.05f * std::sin(rotation.z);

    scene.setTargetPosition(position);
    generateModelMatrix();
    return true;
}

void Boat::render(Scene &scene) {
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
    shader->setUniform("CameraPosition", scene.camera->position + scene.camera->offset);

    mesh->render();
}
