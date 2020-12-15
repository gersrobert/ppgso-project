#include "menu_scene.h"
#include "src/scene/scene_window.h"

void MenuScene::update(float time) {
    Scene::update(time);
    bgColor = {0, 0.1, 0.21875};

    switch (mode) {
        case START:
            if (keyboard[GLFW_KEY_ENTER] == GLFW_PRESS) {
                window.startGame();
            }
            break;
        case PAUSE:
            if (keyboard[GLFW_KEY_P] == GLFW_PRESS) {
                window.pauseGame();
                keyboard[GLFW_KEY_P] = GLFW_RELEASE;
            }
            break;
        default:
            break;
    }
}

void MenuScene::render() {
    Scene::render();
}