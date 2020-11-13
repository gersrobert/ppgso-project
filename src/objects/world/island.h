#ifndef PPGSO_ISLAND_H
#define PPGSO_ISLAND_H

#include <src/scene/object.h>
#include <shader.h>
#include <dependencies/include/GL/glew.h>
#include <src/scene/vertex_object.h>
#include <mesh.h>
#include "chunk.h"

class Island final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;
    uint32_t meshIndex;
    uint32_t textureIndex;

    Chunk &chunk;

public:

    explicit Island(Scene &scene, Chunk &chunk);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_ISLAND_H
