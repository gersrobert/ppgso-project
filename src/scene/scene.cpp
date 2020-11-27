#include "scene.h"

void Scene::update(float time) {
    camera->update(*this, time);

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
}

void Scene::render() {
    // Simply render all objects
    for (auto &obj : objects)
        obj->render(*this);
}

void Scene::setTargetPosition(const glm::vec3 &position, const glm::vec3 &rotation) {
    targetPosition = position;
    camera->position = position + glm::vec3{0, 3, 0};
    camera->rotation = {-std::sin(rotation.z + camera->offset.x), camera->offset.y, -std::cos(rotation.z + camera->offset.z)};
}

std::vector<Object *> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
    std::vector<Object *> intersected = {};
    for (auto &object : objects) {
        // Collision with sphere of size object->scale.x
        auto oc = position - object->position;
        auto radius = object->scale.x;
        auto a = glm::dot(direction, direction);
        auto b = glm::dot(oc, direction);
        auto c = glm::dot(oc, oc) - radius * radius;
        auto dis = b * b - a * c;

        if (dis > 0) {
            auto e = sqrt(dis);
            auto t = (-b - e) / a;

            if (t > 0) {
                intersected.push_back(object.get());
                continue;
            }

            t = (-b + e) / a;

            if (t > 0) {
                intersected.push_back(object.get());
                continue;
            }
        }
    }

    return intersected;
}
