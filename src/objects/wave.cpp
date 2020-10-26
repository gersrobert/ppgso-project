#include "src/scene/scene.h"
#include "wave.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

Wave::Wave() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sea_surface.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("barrel.obj");

    rotation.z = -0.1f * ppgso::PI;
    position.x = 100;
    position.z = 100;
    scale.z *= 10;
    scale.x *= 4;
}

bool Wave::update(Scene &scene, float dt) {
    position.x -= 4 * dt;
    position.z -= 4 * dt;

    generateModelMatrix();
    return true;
}

void Wave::render(Scene &scene) {

    // NOTE: this object does not use camera, just renders the entire quad as is
    shader->use();

    // Pass UV mapping offset to the shader
    shader->setUniform("TextureOffset", textureOffset);

    // Render mesh, not using any projections, we just render in 2D
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("LightDirection", scene.lightDirection);

    mesh->render();
}

// shared resources
std::unique_ptr<ppgso::Mesh> Wave::mesh;
std::unique_ptr<ppgso::Shader> Wave::shader;
std::unique_ptr<ppgso::Texture> Wave::texture;
