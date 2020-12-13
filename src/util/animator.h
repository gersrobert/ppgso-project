#ifndef PPGSO_ANIMATOR_H
#define PPGSO_ANIMATOR_H

#include <glm/vec3.hpp>
#include <src/scene/executable.h>

class Animator : public Executable {
public:
    bool isActive = true;

    Animator(glm::vec3 &animate, glm::vec3 target, float duration);

    bool execute(float delta) override;

private:
    glm::vec3 &animate;
    const glm::vec3 start;
    const glm::vec3 end;
    float duration;
    float elapsed = 0;
};

#endif //PPGSO_ANIMATOR_H
