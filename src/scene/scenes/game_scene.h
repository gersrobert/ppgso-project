#ifndef PPGSO_GAME_SCENE_H
#define PPGSO_GAME_SCENE_H

#include <src/scene/scene.h>

class GameScene : public Scene {
public:
    const float VISIBILITY = 400.0f;

    std::unique_ptr<Camera> camera;
    glm::vec3 targetPosition;
    glm::vec3 targetRotation;
    glm::vec2 windDirection = {1, -2};
    glm::vec3 lightDirection{-1.0f, 1.0f, -1.0f};

    GameScene(): Scene() {}

    void update(float time) override;

    void render() override;

    void setTargetPosition(const glm::vec3 &position, const glm::vec3 &rotation);

};

#endif //PPGSO_GAME_SCENE_H
