#include <cmake-build-debug/shaders/color_vert_glsl.h>
#include <cmake-build-debug/shaders/color_frag_glsl.h>
#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include "wind_vane.h"

std::unique_ptr<ppgso::Mesh> WindVane::mesh;
std::unique_ptr<ppgso::Texture> WindVane::texture;
std::unique_ptr<ppgso::Shader> WindVane::shader;

WindVane::WindVane(Scene &scene, Boat &boat) : boat(boat) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = diffuse_vert_glsl;
        shaderConfig.fs = diffuse_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("wind_vane.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("wind_vane.obj");

    scale *= 0.2;
    scale.y *= 0.5f;
    scale.z *= 1.0f;

    offset = {0, 9.725, 0.525};
}

bool WindVane::update(Scene &scene, float dt) {
    position = boat.position;

    modelMatrix = glm::translate(glm::mat4(1), boat.position + offset)
                  * glm::translate(glm::mat4(1), -offset)
                  * glm::rotate(glm::mat4(1), boat.rotation.z, {0, 1, 0})
                  * glm::rotate(glm::mat4(1), boat.rotation.y, {0, 0, 1})
                  * glm::translate(glm::mat4(1), offset)
                  * glm::rotate(glm::mat4(1), float(-boat.rotation.z + (0.5 * ppgso::PI)), {0, 1, 0})
                  * glm::scale(glm::mat4(1), scale);

    return boat.isActive;
}

void WindVane::render(Scene &scene) {
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
    shader->setUniform("specularFocus", 32.0f);
    shader->setUniform("specularIntensity", 0.5f);
    shader->setUniform("ambientIntensity", 1.0f);
    shader->setUniform("diffuseIntensity", 0.6f);

    mesh->render();
}