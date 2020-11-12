#ifndef PPGSO_BEZIER_H
#define PPGSO_BEZIER_H

#include <dependencies/include/glm/vec3.hpp>
#include <vector>

glm::vec3 lerpFunc(const glm::vec3 &start, const glm::vec3 &end, const float t);

glm::vec3 bezierPoint(const std::vector<glm::vec3> &points, const float t);

#endif //PPGSO_BEZIER_H
