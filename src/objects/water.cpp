#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include "water.h"

Water::Water(Scene &scene) {
    shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_1.bmp"));

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &nbo);

    vertices = {
            {-100,0, -100},
            {-100,0, 100},
            {100, 0, -100},
            {100, 0, 100},
            {-100,-2, -100},
            {-100,-2, 100},
            {100, -2, -100},
            {100, -2, 100},
    };
    texCoords = {
            {0, 0},
            {0, 10},
            {10,  0},
            {10,  10},

//            {0, 0},
//            {0, .5},
//            {.5,  0},
//            {.5,  .5},
//            {.5, .5},
//            {.5, 1},
//            {1,  .5},
//            {1,  1},
    };
    normals = {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
    };
    faces = {
            {4, 5, 6},
            {5, 6, 7},
            {0, 1, 2},
            {1, 2, 3},
    };

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(face), faces.data(), GL_STATIC_DRAW);
}

bool Water::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Water::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", {0, 1, 0});

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("Transparency", 0.5f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}