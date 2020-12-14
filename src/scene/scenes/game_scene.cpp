#include "game_scene.h"
#include "src/scene_window.h"

void GameScene::update(float time) {
    camera->update(*this, time);
    bgColor = {0, 0.1, 0.21875};

    if (keyboard[GLFW_KEY_P] == GLFW_PRESS) {
        window.pauseGame();
        keyboard[GLFW_KEY_P] = GLFW_RELEASE;
    }

    Scene::update(time);
}

void GameScene::render() {
    Scene::render();
}

void GameScene::setTargetPosition(const glm::vec3 &position, const glm::vec3 &rotation) {
    targetPosition = position;
    targetRotation = rotation;

    camera->moveTo(position, rotation);
}