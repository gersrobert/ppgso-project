#include <ppgso.h>
#include "animator.h"

Animator::Animator(glm::vec3 &animate, glm::vec3 target, float duration) : animate(animate), start(animate), end(target), duration(duration) {}

bool Animator::execute(float delta) {
    elapsed += delta;
    if (elapsed >= duration) {
        isActive = false;
        elapsed = duration;
    }

    auto result = glm::lerp(start, end, elapsed / duration);
    animate.x = result.x;
    animate.y = result.y;
    animate.z = result.z;

    return isActive;
}
