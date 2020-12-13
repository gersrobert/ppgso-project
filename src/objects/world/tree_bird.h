#ifndef PPGSO_TREE_BIRD_H
#define PPGSO_TREE_BIRD_H

#include <src/scene/scene.h>
#include <src/scene/object.h>
#include "chunk.h"
#include "tree.h"

class TreeBird final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    Chunk &chunk;
    Tree &tree;

    float time = 0;

public:

    explicit TreeBird(Scene &scene, Chunk &chunk, Tree &tree);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_TREE_BIRD_H
