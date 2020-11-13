#ifndef PPGSO_WATER_H
#define PPGSO_WATER_H

#include <bits/unique_ptr.h>
#include <shader.h>
#include <src/scene/scene.h>
#include <src/scene/vertex_object.h>
#include "chunk.h"

class Water final : public VertexObject {
private:
    static std::unique_ptr<ppgso::Texture> texture;

    Chunk &chunk;

    glm::vec2 textureOffset = {0, 0};
public:

    explicit Water(Scene &scene, Chunk &chunk);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_WATER_H
