#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include "water.h"

Water::Water(Scene &scene) {
    shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_1.bmp"));

    const float size = 1000;
    vertices = {
            {-size,0, -size},
            {-size,0, size},
            {size, 0, -size},
            {size, 0, size},
            {-size,-2, -size},
            {-size,-2, size},
            {size, -2, -size},
            {size, -2, size},
    };
    texCoords = {
            {0, 0},
            {0, size/10},
            {size/10,  0},
            {size/10,  size/10},
            {0, 0},
            {0, size/10},
            {size/10,  0},
            {size/10,  size/10},
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
//            {4, 5, 6},
//            {5, 6, 7},
            {0, 1, 3},
            {0, 3, 2},
    };

    // Copy data to OpenGL
    glBindVertexArray(vao);
    setVertexBuffer();
    setTextureBuffer();
    setNormalBuffer();
    setIndexBuffer();

}

bool Water::update(Scene &scene, float dt) {
    textureOffset.x += 0.0125f * dt;
    textureOffset.y -= 0.025f * dt;

    generateModelMatrix();
    return true;
}

void Water::render(Scene &scene) {
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
    shader->setUniform("TextureOffset", textureOffset);
    shader->setUniform("CameraPosition", scene.camera->position);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}