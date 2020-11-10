#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include "bezier_waves.h"
#include "src/scene/scene.h"


glm::vec3 lerpFunc(const glm::vec3 &start, const glm::vec3 &end, const float t) {
    auto dx = end.x - start.x;
    auto dy = end.y - start.y;
    auto dz = end.z - start.z;

    return glm::vec3{
            start.x + (dx * t),
            start.y + (dy * t),
            start.z + (dz * t),
    };
}

glm::vec3 bezierPoint(const std::vector<glm::vec3> &points, const float t) {
    std::vector<glm::vec3> subPoints = {points};
    while (subPoints.size() > 1) {
        std::vector<glm::vec3> newPoints;
        for (size_t i = 1; i < subPoints.size(); i++) {
            newPoints.push_back(lerpFunc(subPoints.at(i - 1), subPoints.at(i), t));
        }
        subPoints = newPoints;
    }
    return subPoints.at(0);
}

BezierWaves::BezierWaves() {
    // Initialize static resources if needed
    shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_1.bmp"));

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &nbo);

    for (size_t i = 0; i < CP_LEN; ++i) {
        std::vector<glm::vec3> row = {};
        for (size_t j = 0; j < CP_LEN; ++j) {
            float x = float(j) * (2.0f / (CP_LEN - 1)) - 1.0f;
            float y = 0;
            float z = float(i) * (2.0f / (CP_LEN - 1)) - 1.0f;
            row.emplace_back(x * 50, y * 50, z * 50);
        }
        controlPointsBase.push_back(row);
    }

    scale *= 10;
}

bool BezierWaves::update(Scene &scene, float dt) {
    vertices = {};
    texCoords = {};
    mesh = {};
    normals = {};

    // Generate Bezier patch points and incidences
    std::vector<std::vector<glm::vec3>> controlPoints;
    {
        static float offset = 0;
        offset += dt;

        for (size_t i = 0; i < controlPointsBase.size(); ++i) {
            std::vector<glm::vec3> row;
            for (size_t j = 0; j < controlPointsBase.at(i).size(); ++j) {
                float x = controlPointsBase.at(i).at(j).x;
                float y = controlPointsBase.at(i).at(j).y;
                float z = controlPointsBase.at(i).at(j).z;

                if (i % 4 == 0) {
                    y = std::sin(offset) * 1.0f;
                } else if (i % 4 == 2) {
                    y = std::cos(offset) * 1.0f;
                }

                row.emplace_back(x, y, z);
            }
            controlPoints.push_back(row);
        }
    }

    unsigned int PATCH_SIZE = 24;
    for (unsigned int i = 0; i < PATCH_SIZE; i++) {
        std::vector<glm::vec3> points = {};
        for (uint32_t u = 0; u < CP_LEN; ++u) {
            points.push_back(
                    bezierPoint(controlPoints.at(u), static_cast<float>(i) / static_cast<float>(PATCH_SIZE - 1)));
        }

        for (unsigned int j = 0; j < PATCH_SIZE; j++) {
            auto point = bezierPoint(points, static_cast<float>(j) / static_cast<float>(PATCH_SIZE - 1));

            vertices.push_back(point);
            texCoords.emplace_back(glm::vec2{point.x / 10, point.z / 10});
            normals.emplace_back(0, 1, 0);
        }
    }
    // Generate indices
    for (unsigned int i = 1; i < PATCH_SIZE; i++) {
        for (unsigned int j = 1; j < PATCH_SIZE; j++) {
            mesh.push_back({
                                   (i - 1) * PATCH_SIZE + (j - 1),
                                   (i - 1) * PATCH_SIZE + (j),
                                   (i) * PATCH_SIZE + (j - 1),
                           });

            mesh.push_back({
                                   (i) * PATCH_SIZE + (j),
                                   (i - 1) * PATCH_SIZE + (j),
                                   (i) * PATCH_SIZE + (j - 1),
                           });
        }
    }

    // Copy data to OpenGL
    glBindVertexArray(vao);

    // Copy positions to gpu
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = shader->getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Copy texture positions to gpu
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto texCoord_attrib = shader->getAttribLocation("TexCoord");
    glEnableVertexAttribArray(texCoord_attrib);
    glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(),
                 GL_STATIC_DRAW);

    auto normalCoord_attrib = shader->getAttribLocation("Normal");
    glEnableVertexAttribArray(normalCoord_attrib);
    glVertexAttribPointer(normalCoord_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Copy indices to gpu
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(face), mesh.data(), GL_STATIC_DRAW);

    generateModelMatrix();
    return true;
}

void BezierWaves::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("Transparency", 0.5f);

    glBindVertexArray(vao);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh.size() * 3, GL_UNSIGNED_INT, nullptr);
}
