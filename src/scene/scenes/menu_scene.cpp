#include "menu_scene.h"

void MenuScene::update(float time) {
    Scene::update(time);
    bgColor = {0, 0.1, 0.21875};
}

void MenuScene::render() {
    Scene::render();
}