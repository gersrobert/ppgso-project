#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fov, float ratio, float near, float far) {
    float fovInRad = (ppgso::PI / 180.0f) * fov;

    projectionMatrix = glm::perspective(fovInRad, ratio, near, far);
}

void Camera::update() {
    viewMatrix = glm::lookAt(position - offset, position - back, up);
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
