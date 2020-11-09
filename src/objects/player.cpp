#include "player.h"
#include "src/scene/scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
    // Scale the default model
    scale *= 1.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Sailboat_01.obj");

    position.y -= 0.4f;
    rotation.y =  -0.15f;
}

bool Player::update(Scene &scene, float dt) {
    // Fire delay increment

    // Hit detection
    for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;
    }

    // Keyboard controls
    float deltaRotation = 0;
    if (scene.keyboard[GLFW_KEY_D]) {
        deltaRotation = -0.5f * dt;
    }
    if (scene.keyboard[GLFW_KEY_A]) {
        deltaRotation = 0.5f * dt;
    }

    rotation.z += deltaRotation;
    position.z += 0.1f * std::cos(rotation.z);
    position.x += 0.1f * std::sin(rotation.z);
    scene.camera->position = position;

    generateModelMatrix();
    return true;
}

void Player::render(Scene &scene) {
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

void Player::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}
