#ifndef PPGSO_TREE_H
#define PPGSO_TREE_H

#include <src/scene/scene.h>
#include <src/scene/object.h>
#include "chunk.h"
#include "island.h"

class Tree final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    Chunk &chunk;
    Island &island;

public:

    explicit Tree(Scene &scene, Chunk &chunk, Island &island);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_TREE_H
