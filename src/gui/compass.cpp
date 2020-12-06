#include <cmake-build-debug/shaders/color_vert_glsl.h>
#include <cmake-build-debug/shaders/color_frag_glsl.h>
#include <src/scene/scenes/game_scene.h>
#include "compass.h"
#include "compass_hand.h"

std::unique_ptr<ppgso::Shader> Compass::shader;

Compass::Compass(Scene &scene) : VertexObject() {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = color_vert_glsl;
        shaderConfig.fs = color_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }

    auto hand = std::make_unique<CompassHand>(scene);
    scene.guiObjects.push_back(move(hand));

    vertices = {
            {-0.1, 1, 0},
            {0.1, 1, 0},
            {0.0, 0.7, 0},

            {-0.1, -1, 0},
            {0.1, -1, 0},
            {0.0, -0.7, 0},

            {-1, -0.1, 0},
            {-1, 0.1, 0},
            {-0.7, 0.0, 0},

            {1, -0.1, 0},
            {1, 0.1, 0},
            {0.7, 0.0, 0},
    };

    faces = {
            {2, 1, 0},
            {5, 3, 4},
            {6, 8, 7},
            {9, 10, 11},
    };

    colors = {
            {1, 0.2f, 0.2f, 1},
            {1, 0.2f, 0.2f, 1},
            {1, 0.2f, 0.2f, 1},

            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1},

            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1},

            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1},
    };

    glBindVertexArray(vao);
    setVertexBuffer(shader);
    setIndexBuffer();

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = shader->getAttribLocation("Color");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    scale *= 0.07;
    position.y = -.92f;
}

Compass::~Compass() {
    glDeleteBuffers(1, &cbo);
}

bool Compass::update(Scene &scene, float dt) {
    auto gameScene = dynamic_cast<GameScene*>(&scene);

    modelMatrix = glm::translate(glm::mat4(1), position)
            * glm::rotate(glm::mat4(1), gameScene->targetRotation.z, {0, 0, -1})
            * glm::scale(glm::mat4(1), scale);

    return isActive;
}

void Compass::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.guiProjection);
    shader->setUniform("ViewMatrix", glm::mat4(1));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", {0, 0, 0, 0});

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}