#include <glm/glm.hpp>
#include <src/util/animator.h>
#include "camera.h"
#include "scene.h"

Camera::Camera(float fov, float ratio, float near, float far) {
    float fovInRad = (ppgso::PI / 180.0f) * fov;

    projectionMatrix = glm::perspective(fovInRad, ratio, near, far);
}

void Camera::moveThirdPerson(Scene &scene, float time) {
//    distance = 10;
    positionOffset = {0, 3, 0};

    static float camLeftRight = 0;
    if (scene.keyboard[GLFW_KEY_RIGHT]) {
        camLeftRight += 0.75f * time;
        offset.x = camLeftRight;
        offset.z = camLeftRight;
    }
    if (scene.keyboard[GLFW_KEY_LEFT]) {
        camLeftRight -= 0.75f * time;
        offset.x = camLeftRight;
        offset.z = camLeftRight;
    }
    if (scene.keyboard[GLFW_KEY_UP]) {
        offset.y += 0.01f;
    }
    if (scene.keyboard[GLFW_KEY_DOWN]) {
        offset.y -= 0.01f;
        offset.y = std::max(-0.15f, offset.y);
    }
}

void Camera::update(Scene &scene, float time) {
    if (!initialized) {
        initialize(scene);
    }

    if (scene.keyboard[GLFW_KEY_V]) {
        cameraMode = THIRD_PERSON;
        distance = -10;
    } else {
        cameraMode = THIRD_PERSON;
        distance = 10;
    }

    switch (cameraMode) {
        case THIRD_PERSON:
            moveThirdPerson(scene, time);
            break;
        case CINEMATIC:
            break;
    }

    viewMatrix = glm::lookAt(getTotalPosition(), position - back, up);
}

glm::vec3 Camera::cast(double u, double v) {
    // Create point in Screen coordinates
    glm::vec4 screenPosition{u, v, 0.0f, 1.0f};

    // Use inverse matrices to get the point in world coordinates
    auto invProjection = glm::inverse(projectionMatrix);
    auto invView = glm::inverse(viewMatrix);

    // Compute position on the camera plane
    auto planePosition = invView * invProjection * screenPosition;
    planePosition /= planePosition.w;

    // Create direction vector
    auto direction = glm::normalize(planePosition - glm::vec4{offset, 1.0f});
    return glm::vec3{direction};
}

glm::vec3 Camera::getTotalPosition() const {
    return (position) + (distance * (rotation));
}

void Camera::moveTo(const glm::vec3 &pos, const glm::vec3 &rot) {
    position = pos + positionOffset;
    rotation = {-std::sin(rot.z + offset.x), offset.y, -std::cos(rot.z + offset.z)};
}

void Camera::initialize(Scene &scene) {
    initialized = true;

    offset.y = 20;
    auto animator = std::make_unique<Animator>(offset, glm::vec3{0, 0, 0}, 2.0f);
    scene.executables.push_back(move(animator));
}
