#include <cmake-build-debug/shaders/color_vert_glsl.h>
#include <cmake-build-debug/shaders/color_frag_glsl.h>
#include "compass_hand.h"

std::unique_ptr<ppgso::Shader> CompassHand::shader;

CompassHand::CompassHand(Scene &scene) : VertexObject() {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = color_vert_glsl;
        shaderConfig.fs = color_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }

    vertices = {
            {0, -1, 0.1},
            {-0.2, 0, 0.1},
            {0.2, 0, 0.1},
            {0, 1, 0.1},

            {-2.0, -2.0, -0.1},
            {-2.0, 2.0, -0.1},
            {2.0, -2.0, -0.1},
            {2.0, 2.0, -0.1},
    };
    faces = {
            {0, 2, 1},
            {3, 1, 2},

            {4, 6, 5},
            {6, 7, 5},
    };
    colors = {
            {1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},

            {0.1f, 0.1f, 0.1f, 0.5f},
            {0.1f, 0.1f, 0.1f, 0.5f},
            {0.1f, 0.1f, 0.1f, 0.5f},
            {0.1f, 0.1f, 0.1f, 0.5f},
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

    position.y = -.92f;
    scale *= 0.04f;
}

CompassHand::~CompassHand() {
    glDeleteBuffers(1, &cbo);
}

bool CompassHand::update(Scene &scene, float dt) {
    modelMatrix = glm::translate(glm::mat4(1), position)
                  * glm::scale(glm::mat4(1), scale);

    return isActive;
}

void CompassHand::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.guiProjection);
    shader->setUniform("ViewMatrix", glm::mat4(1));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", {0, 0, 0, 0});

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}
