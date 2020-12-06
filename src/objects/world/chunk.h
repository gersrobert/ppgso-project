#ifndef PPGSO_CHUNK_H
#define PPGSO_CHUNK_H

#include <src/scene/object.h>
#include <src/scene/scenes/game_scene.h>

class Chunk final : public Object {
private:
    bool isOutOfBounds(glm::vec3 pos);

public:
    const float size;
    const glm::vec2 position;
    bool isActive = true;

    Chunk(GameScene &scene, glm::vec2 position);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_CHUNK_H
