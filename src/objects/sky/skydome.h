#ifndef PPGSO_SKYDOME_H
#define PPGSO_SKYDOME_H

#include <ppgso.h>
#include <src/scene/object.h>
#include <src/scene/scene.h>
#include <src/scene/scenes/game_scene.h>

class Skydome final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    explicit Skydome(GameScene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_SKYDOME_H
