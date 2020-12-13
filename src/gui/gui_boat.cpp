#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <ppgso.h>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include "gui_boat.h"

std::unique_ptr<ppgso::Shader> GuiBoat::shader;
std::unique_ptr<ppgso::Mesh> GuiBoat::mesh;
std::unique_ptr<ppgso::Texture> GuiBoat::texture;

GuiBoat::GuiBoat(Scene &scene) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = color_vert_glsl;
        shaderConfig.fs = color_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("orange_boat.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("orange_boat.obj");

    scale *= 0.1;
    position.y = -0.5f;

    auto startMatrix = glm::translate(glm::mat4(1), position)
                       * glm::rotate(glm::mat4(1), 0.0f, {0, 1, 0})
                       * glm::scale(glm::mat4(1), scale);

    auto endMatrix = glm::translate(glm::mat4(1), position)
                       * glm::rotate(glm::mat4(1), 180.0f * ppgso::PI/180.0f, {0, 1, 0})
                       * glm::scale(glm::mat4(1), scale);

    initInterpolation(startMatrix, endMatrix, 10.0f, 0);
}

bool GuiBoat::update(Scene &scene, float dt) {
    modelMatrix = interpolate(dt);
    return isActive;
}

void GuiBoat::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.guiProjection);
    shader->setUniform("ViewMatrix", glm::mat4(1));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("OverallColor", {0.8, 0.8, 0.8, 1});
    mesh->render();
}
