#include "bezier.h"

glm::vec3 lerpFunc(const glm::vec3 &start, const glm::vec3 &end, const float t) {
    auto dx = end.x - start.x;
    auto dy = end.y - start.y;
    auto dz = end.z - start.z;

    return glm::vec3{
            start.x + (dx * t),
            start.y + (dy * t),
            start.z + (dz * t),
    };
}

glm::vec3 bezierPoint(const std::vector<glm::vec3> &points, const float t) {
    std::vector<glm::vec3> subPoints = {points};
    while (subPoints.size() > 1) {
        std::vector<glm::vec3> newPoints;
        for (size_t i = 1; i < subPoints.size(); i++) {
            newPoints.push_back(lerpFunc(subPoints.at(i - 1), subPoints.at(i), t));
        }
        subPoints = newPoints;
    }
    return subPoints.at(0);
}