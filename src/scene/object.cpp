#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <iostream>

#include "object.h"

void Object::generateModelMatrix() {
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Object::interpolate(float dt) {
    interpTime += dt;

    if (interpTime >= interpDuration) {
        if (interpBehavior == 0) {
            return interpEnd;
        } else if (interpBehavior == 1) {
            interpTime = 0;
        }
    }

    return glm::interpolate(interpStart, interpEnd, interpTime / interpDuration);
}

void Object::initInterpolation(glm::mat4 start, glm::mat4 end, float duration, int behavior) {
    interpStart = start;
    interpEnd = end;
    interpDuration = duration;
    interpBehavior = behavior;
}

void Object::translateTo(glm::vec3 pos) {
    position = pos;
}

void Object::translateBy(glm::vec3 pos) {
    position += pos;
}

void Object::rotateTo(glm::vec3 rot) {
    rotation = rot;
}

void Object::rotateBy(glm::vec3 rot) {
    rotation += rot;
}
