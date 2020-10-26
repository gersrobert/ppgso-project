#include "space.h"
#include "src/scene/scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

Space::Space() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sea_surface.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("plane.obj");

    rotation.x = ppgso::PI;
    scale *= 1000;
}

bool Space::update(Scene &scene, float dt) {
    // Offset for UV mapping, creates illusion of scrolling
//    textureOffset.y -= dt / 5;

    generateModelMatrix();
    return true;
}

void Space::render(Scene &scene) {

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
std::unique_ptr<ppgso::Mesh> Space::mesh;
std::unique_ptr<ppgso::Shader> Space::shader;
std::unique_ptr<ppgso::Texture> Space::texture;
