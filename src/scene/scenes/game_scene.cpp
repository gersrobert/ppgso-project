#include "game_scene.h"

void GameScene::update(float time) {
    camera->update(*this, time, targetPosition, targetRotation);

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);

    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, time))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }

    i = std::begin(guiObjects);
    while (i != std::end(guiObjects)) {
        auto obj = i->get();
        if (!obj->update(*this, time))
            i = guiObjects.erase(i);
        else
            ++i;
    }
}

void GameScene::render() {
    for (auto &obj : objects) {
        obj->render(*this);
    }
    for (auto &obj : guiObjects) {
        obj->render(*this);
    }
}

void GameScene::setTargetPosition(const glm::vec3 &position, const glm::vec3 &rotation) {
    targetPosition = position;
    targetRotation = rotation;
}