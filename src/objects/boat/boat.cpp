#include "boat.h"
#include "src/scene/scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <src/scene/scenes/game_scene.h>
#include <src/objects/world/island.h>
#include <src/util/animator.h>
#include <src/gui/screen_overlay.h>
#include "boat_wheel.h"
#include "mainsail.h"
#include "foresail.h"
#include "wind_vane.h"
#include "../world/lighthouse.h"

// shared resources
std::unique_ptr<ppgso::Mesh> Boat::mesh;
std::unique_ptr<ppgso::Texture> Boat::texture;
std::unique_ptr<ppgso::Shader> Boat::shader;

Boat::Boat(Scene &scene) {
    // Scale the default model
    scale *= 0.5f;

    // Initialize static resources if needed
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("orange_boat.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("orange_boat_no_wheel_no_sails.obj");

    auto wheel = std::make_unique<BoatWheel>(scene, *this);
    scene.objects.push_back(move(wheel));
    auto mainsail = std::make_unique<Mainsail>(scene, *this);
    scene.objects.push_back(move(mainsail));
    auto foresail = std::make_unique<Foresail>(scene, *this);
    scene.objects.push_back(move(foresail));
    auto vane = std::make_unique<WindVane>(scene, *this);
    scene.objects.push_back(move(vane));

    position.y -= 1.1f;
    rotation.x = -0.015f;
}

float Boat::calculateSailEffect(Scene &scene, float dt) {
    if (scene.keyboard[GLFW_KEY_W]) {
        sailSheathe += dt * 0.33f;
    }
    if (scene.keyboard[GLFW_KEY_S]) {
        sailSheathe -= dt * 0.33f;
    }
    if (sailSheathe > 1) {
        sailSheathe = 1;
    }
    if (sailSheathe < 0) {
        sailSheathe = 0;
    }

    float sailEffect = std::abs(std::cos(rotation.z)) - sailSheathe;
    if (sailEffect < 0) {
        sailEffect = -1;
    } else {
        sailEffect *= 1 + (20.0f * ppgso::PI/180.0f);
        sailEffect -= 20.0f * ppgso::PI/180.0f;
    }
    return sailEffect;
}

float Boat::calculateSpeed(float currentSpeed, float sailForce) {
    float stoppingForce = (0.9f * currentSpeed + 0.1f);

    float acceleration = std::cos(sailForce * ppgso::PI/2) - stoppingForce;

    return currentSpeed + 0.001f * acceleration;
}

void Boat::checkCollisions(Scene &scene, float dt) {
    for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;

        auto island = dynamic_cast<Island*>(obj.get());
        if (island) {
            static bool collisionStarted = false;
            if (glm::distance(this->position, island->position) < (this->scale.x + island->scale.x) * 0.15f) {
                mode = COLLISION;

                if (!collisionStarted) {
                    collisionStarted = true;

                    auto overlay = std::make_unique<ScreenOverlay>(scene, mode);
                    scene.guiObjects.push_back(move(overlay));
                }
            }
        }

        auto lighthouse = dynamic_cast<LightHouse*>(obj.get());
        if (lighthouse) {
            static bool collisionStarted = false;
            if (glm::distance(this->position, lighthouse->position) < (this->scale.x + lighthouse->scale.x) * 7.0f) {
                mode = END;

                if (!collisionStarted) {
                    collisionStarted = true;

                    auto overlay = std::make_unique<ScreenOverlay>(scene, mode);
                    scene.guiObjects.push_back(move(overlay));
                }
            }
        }
    }
}

bool Boat::update(Scene &scene, float dt) {
    checkCollisions(scene, dt);

    switch (mode) {
        case GAME:
        case END:
            updateGame(scene, dt);
            break;
        case COLLISION:
            updateCollision(scene, dt);
            break;
    }
}

bool Boat::updateGame(Scene &scene, float dt) {
    float sailEffect = calculateSailEffect(scene, dt);
    speed = calculateSpeed(speed, sailEffect);

    static int xt = 0;
    xt++;
    if (xt % 144 == 0) {
        std::cout << speed << ", " << sailEffect << std::endl;
    }

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

    rotation.z += std::fmod(rotationSpeed * speed, 2*ppgso::PI);

    position.z += 0.05f * speed * std::cos(rotation.z);
    position.x += 0.05f * speed * std::sin(rotation.z);

    rotation.y = 0.15f * std::cos(rotation.z);

    dynamic_cast<GameScene*>(&scene)->setTargetPosition(position, rotation);

    generateModelMatrix();
    return isActive;
}

bool Boat::updateCollision(Scene &scene, float dt) {
    auto animator = std::make_unique<Animator>(position, position + glm::vec3{0, -3, 0}, 6);
    scene.executables.push_back(move(animator));

    generateModelMatrix();
    return isActive;
}

void Boat::render(Scene &scene) {
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
    shader->setUniform("specularFocus", 64.0f);
    shader->setUniform("specularIntensity", 1.0f);

    mesh->render();
}
