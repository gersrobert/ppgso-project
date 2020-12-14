#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include "screen_overlay.h"
#include "src/scene_window.h"

std::unique_ptr<ppgso::Shader> ScreenOverlay::shader;

ScreenOverlay::ScreenOverlay(Scene &scene, Boat::Mode mode) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = color_vert_glsl;
        shaderConfig.fs = color_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }

    vertices = {
            {-2, -1, 1},
            {-2, 1, 1},
            {2, -1, 1},
            {2, 1, 1},
    };

    faces = {
            {0, 2, 1},
            {2, 3, 1},
    };

    glBindVertexArray(vao);
    setVertexBuffer(shader);
    setIndexBuffer();
}

bool ScreenOverlay::update(Scene &scene, float dt) {
    color.a += dt;
    if (color.a >= 1) {
        scene.window.endGame();
    }

    generateModelMatrix();
    return isActive;
}

void ScreenOverlay::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.guiProjection);
    shader->setUniform("ViewMatrix", glm::mat4(1));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", color);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}
