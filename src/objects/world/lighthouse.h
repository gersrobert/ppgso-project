#ifndef PPGSO_LIGHTHOUSE_H
#define PPGSO_LIGHTHOUSE_H

#include <src/scene/object.h>
#include <shader.h>
#include <mesh.h>
#include "chunk.h"
#include "island.h"

class LightHouse final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    explicit LightHouse(Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_LIGHTHOUSE_H
