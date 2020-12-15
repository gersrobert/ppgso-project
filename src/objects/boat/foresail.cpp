#include <cmake-build-debug/shaders/foresail_diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <dependencies/include/glm/gtx/euler_angles.hpp>
#include <src/scene/scenes/game_scene.h>
#include "foresail.h"

std::unique_ptr<ppgso::Mesh> Foresail::mesh;
std::unique_ptr<ppgso::Texture> Foresail::texture;
std::unique_ptr<ppgso::Shader> Foresail::shader;

Foresail::Foresail(Scene &scene, Boat &boat) : boat(boat) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = foresail_diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("orange_boat.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("orange_boat_foresail.obj");

    scale = boat.scale;
    offset = {0, 2, 0.7};
}

bool Foresail::update(Scene &scene, float dt) {
    translateTo(boat.position + offset);
    rotateTo(boat.rotation);

    modelMatrix = glm::translate(glm::mat4(1), position)
                  * glm::translate(glm::mat4(1), -offset)
                  * glm::rotate(glm::mat4(1), boat.rotation.z, {0, 1, 0})
                  * glm::rotate(glm::mat4(1), boat.rotation.y, {0, 0, 1})
                  * glm::rotate(glm::mat4(1), boat.rotation.x, {1, 0, 0})
                  * glm::translate(glm::mat4(1), offset)
                  * glm::scale(glm::mat4(1), scale);

    return boat.isActive;
}

void Foresail::render(Scene &scene) {
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
    shader->setUniform("sailRotation", std::max(-boat.sailSheathe * 1.0f, std::min(std::cos(boat.rotation.z), boat.sailSheathe * 1.0f)));
    shader->setUniform("specularIntensity", 0.0f);
    shader->setUniform("ambientIntensity", 0.5f);
    shader->setUniform("diffuseIntensity", 0.8f);

    mesh->render();
}