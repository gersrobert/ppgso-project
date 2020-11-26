#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <dependencies/include/glm/gtx/euler_angles.hpp>
#include "boat_wheel.h"


std::unique_ptr<ppgso::Mesh> BoatWheel::mesh;
std::unique_ptr<ppgso::Texture> BoatWheel::texture;
std::unique_ptr<ppgso::Shader> BoatWheel::shader;

BoatWheel::BoatWheel(Scene &scene, Boat &boat) : boat(boat) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("orange_boat.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("wheel.obj");

    scale = boat.scale;
    offset = {0, 1.7, -2.15};
}

bool BoatWheel::update(Scene &scene, float dt) {
    position = boat.position + offset;
    rotation = boat.rotation;

    modelMatrix = glm::translate(glm::mat4(1), position)
                  * glm::translate(glm::mat4(1), -offset)
                  * glm::rotate(glm::mat4(1), boat.rotation.z, {0, 1, 0})
                  * glm::rotate(glm::mat4(1), boat.rotation.y, {0, 0, 1})
                  * glm::translate(glm::mat4(1), offset)
                  * glm::rotate(glm::mat4(1), -boat.rotationSpeed * 1000, {0, 0, 1})
                  * glm::scale(glm::mat4(1), scale);

    return boat.isActive;
}

void BoatWheel::render(Scene &scene) {
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
    shader->setUniform("CameraPosition", scene.camera->getTotalPosition());

    mesh->render();
}