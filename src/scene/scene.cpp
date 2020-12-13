#include "scene.h"

void Scene::update(float time) {
    // Use iterator to update all objects so we can remove while iterating
    auto i_e = std::begin(executables);
    while (i_e != std::end(executables)) {
        // Update and remove from list if needed
        auto obj = i_e->get();
        if (!obj->execute(time))
            i_e = executables.erase(i_e); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i_e;
    }

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

void Scene::render() {
    for (auto &obj : objects) {
        obj->render(*this);
    }
    for (auto &obj : guiObjects) {
        obj->render(*this);
    }
}
