#ifndef PPGSO_GUI_BOAT_H
#define PPGSO_GUI_BOAT_H

#include <src/scene/object.h>
#include <src/scene/scene.h>
#include <shader.h>
#include <mesh.h>

class GuiBoat final : public Object {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    explicit GuiBoat(Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_GUI_BOAT_H
