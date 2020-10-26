#include "player.h"
#include "src/scene/scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
  // Scale the default model
  scale *= 1.0f;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;

  // Hit detection
  for ( auto& obj : scene.objects ) {
    // Ignore self in scene
    if (obj.get() == this)
      continue;
  }

  // Keyboard controls
  if(scene.keyboard[GLFW_KEY_W]) {
      position.z += 0.1f;
      scene.camera->position.z += 0.1f;
  }
  if(scene.keyboard[GLFW_KEY_S]) {
      position.z -= 0.1f;
      scene.camera->position.z -= 0.1f;
  }
  if(scene.keyboard[GLFW_KEY_D]) {
      position.x -= 0.1f;
      scene.camera->position.x -= 0.1f;
  }
  if (scene.keyboard[GLFW_KEY_A]) {
      position.x += 0.1f;
      scene.camera->position.x += 0.1f;
  }
  scene.camera->center = position;

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Player::onClick(Scene &scene) {
  std::cout << "Player has been clicked!" << std::endl;
}
