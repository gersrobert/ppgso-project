#include <cmake-build-debug/shaders/diffuse_vert_glsl.h>
#include <cmake-build-debug/shaders/diffuse_frag_glsl.h>
#include <cmake-build-debug/shaders/texture_vert_glsl.h>
#include <cmake-build-debug/shaders/texture_frag_glsl.h>
#include "water.h"

std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> VertexObject::shader;

Water::Water(Scene &scene, Chunk &chunk) : chunk(chunk) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_1.bmp"));

    vertices = {
            {
                    -chunk.size + chunk.size * chunk.position.x * 2.0,
                    0,
                    -chunk.size + chunk.size * chunk.position.y * 2.0
            },
            {       -chunk.size + chunk.size * chunk.position.x * 2.0,
                    0,
                    chunk.size + chunk.size * chunk.position.y * 2.0
            },
            {       chunk.size + chunk.size * chunk.position.x * 2.0,
                    0,
                    -chunk.size + chunk.size * chunk.position.y * 2.0
            },
            {       chunk.size + chunk.size * chunk.position.x * 2.0,
                    0,
                    chunk.size + chunk.size * chunk.position.y * 2.0
            },
    };
    texCoords = {
            {0,               0},
            {0,               chunk.size / 10},
            {chunk.size / 10, 0},
            {chunk.size / 10, chunk.size / 10},
    };
    normals = {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
    };
    faces = {
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
    textureOffset.x += scene.windDirection.x * 0.0125f * dt;
    textureOffset.y += scene.windDirection.y * 0.0125f * dt;

    generateModelMatrix();
    return chunk.isActive;
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
    shader->setUniform("Transparency", 0.75f);
    shader->setUniform("TextureOffset", textureOffset);
    shader->setUniform("CameraPosition", scene.camera->position);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces.size() * 3, GL_UNSIGNED_INT, nullptr);
}