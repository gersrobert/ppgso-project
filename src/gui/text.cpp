#include "text.h"
#include <cmake-build-debug/shaders/text_vert_glsl.h>
#include <cmake-build-debug/shaders/text_frag_glsl.h>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

std::unique_ptr<ppgso::Shader> Text::shader;
std::unique_ptr<ppgso::Texture> Text::texture;

Text::Text(Scene &scene, const std::string &text, const glm::vec4 &color) : VertexObject(), text(text) {
    if (!shader) {
        ppgso::ShaderConfig shaderConfig;
        shaderConfig.vs = text_vert_glsl;
        shaderConfig.fs = text_frag_glsl;
        shader = std::make_unique<ppgso::Shader>(shaderConfig);
    }
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("font.bmp"));

    float nextX = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        auto c = charConfig.at(text.at(i));

        vertices.emplace_back(nextX, c.yOffset, 0);
        vertices.emplace_back(nextX + 1, c.yOffset, 0);
        vertices.emplace_back(nextX, -1 + c.yOffset, 0);
        vertices.emplace_back(nextX + 1, -1 + c.yOffset, 0);
        nextX += c.width;

        texCoords.emplace_back(c.u - 0.002, c.v - 0.002);
        texCoords.emplace_back(c.u + 0.098, c.v - 0.002);
        texCoords.emplace_back(c.u - 0.002, c.v + 0.098);
        texCoords.emplace_back(c.u + 0.098, c.v + 0.098);

        colors.push_back(color);
        colors.push_back(color);
        colors.push_back(color);
        colors.push_back(color);

        faces.push_back({(GLuint) (i * 4 + 0), (GLuint) (i * 4 + 2), (GLuint) (i * 4 + 3)});
        faces.push_back({(GLuint) (i * 4 + 3), (GLuint) (i * 4 + 1), (GLuint) (i * 4 + 0)});
    }

    glBindVertexArray(vao);
    setVertexBuffer(shader);
    setTextureBuffer(shader);
    setIndexBuffer();

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);

    auto position_attrib = shader->getAttribLocation("Color");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    scale *= 0.1f;
}

bool Text::update(Scene &scene, float dt) {
    modelMatrix = glm::translate(glm::mat4(1), position) * glm::scale(glm::mat4(1), scale);

    return isActive;
}

void Text::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.guiProjection);
    shader->setUniform("ViewMatrix", glm::mat4(1));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("OverallColor", {1, 1, 1, 1});

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}
