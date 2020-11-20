#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <dependencies/include/glm/gtx/euler_angles.hpp>
#include "mainsail.h"

std::unique_ptr<ppgso::Mesh> Mainsail::mesh;
std::unique_ptr<ppgso::Texture> Mainsail::texture;
std::unique_ptr<ppgso::Shader> Mainsail::shader;

Mainsail::Mainsail(Scene &scene, Boat &boat) : boat(boat) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("orange_boat.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("orange_boat_mainsail.obj");

    scale = boat.scale;
    offset = {0, 2.810, 0.645};
    spin = -0.2f;
}

bool Mainsail::update(Scene &scene, float dt) {
    modelMatrix = glm::translate(glm::mat4(1), boat.position + offset)
                  * glm::translate(glm::mat4(1), -offset)
                  * glm::rotate(glm::mat4(1), boat.rotation.z, {0, 1, 0})
                  * glm::rotate(glm::mat4(1), boat.rotation.y, {0, 0, 1})
                  * glm::rotate(glm::mat4(1), boat.rotation.x, {1, 0, 0})
                  * glm::translate(glm::mat4(1), offset)
                  * glm::rotate(glm::mat4(1), spin, {0, 1, 0})
                  * glm::scale(glm::mat4(1), scale);

    return boat.isActive;
}

void Mainsail::render(Scene &scene) {
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