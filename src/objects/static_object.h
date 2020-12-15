#ifndef PPGSO_STATIC_OBJECT_H
#define PPGSO_STATIC_OBJECT_H

#include <src/scene/object.h>
#include <shader.h>
#include <dependencies/include/GL/glew.h>
#include <mesh.h>

class StaticObject final : public Object {
private:
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;

public:

    explicit StaticObject(Scene &scene, const std::string &meshName, const std::string &textureName);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_STATIC_OBJECT_H
