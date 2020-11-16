#ifndef PPGSO_CLOUD_H
#define PPGSO_CLOUD_H

#include <src/scene/object.h>
#include <src/scene/scene.h>

class Cloud final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    explicit Cloud(Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_CLOUD_H
