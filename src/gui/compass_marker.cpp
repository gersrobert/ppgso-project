#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <glm/gtx/vector_angle.hpp>
#include "compass_marker.h"
#include "src/scene/scenes/game_scene.h"

std::unique_ptr<ppgso::Shader> CompassMarker::shader;

CompassMarker::CompassMarker(Scene &scene) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = color_vert_glsl;
        shaderConfig.fs = color_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }

    vertices = {
            {-0.2, -0.2, 0.1},
            {-0.2, 0.2,  0.1},
            {0.2,  -0.2, 0.1},
            {0.2,  0.2,  0.1},
    };
    faces = {
            {0, 2, 1},
            {3, 1, 2},
    };

    glBindVertexArray(vao);
    setVertexBuffer(shader);
    setIndexBuffer();

    position.y = -.92f;
    scale *= 0.04f;
}

bool CompassMarker::update(Scene &scene, float dt) {
    auto gameScene = dynamic_cast<GameScene*>(&scene);
    auto angle = glm::orientedAngle(
            glm::normalize(glm::vec2{std::sin(gameScene->targetRotation.z), std::cos(gameScene->targetRotation.z)}),
            glm::normalize(glm::vec2(gameScene->lightHousePosition.x - gameScene->targetPosition.x, gameScene->lightHousePosition.y - gameScene->targetPosition.z))
    );

    modelMatrix = glm::translate(glm::mat4(1), position)
                  * glm::rotate(glm::mat4(1), -angle, {0, 0, 1})
                  * glm::translate(glm::mat4(1), {0, 0.065, 0})
                  * glm::scale(glm::mat4(1), scale);
    return isActive;
}

void CompassMarker::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.guiProjection);
    shader->setUniform("ViewMatrix", glm::mat4(1));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", {0.1, 0.4, 0.8, 1.0});

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}
