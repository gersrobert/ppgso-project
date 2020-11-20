//
// Created by Robert Gers on 09/11/2020.
//

#ifndef PPGSO_RUDDER_H
#define PPGSO_RUDDER_H


#include <src/scene/object.h>
#include <texture.h>
#include <mesh.h>
#include <shader.h>
#include "src/objects/boat/boat.h"

class Rudder final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    Boat &boat;
    bool steeringLeft = false;
    bool steeringRight = false;

public:

    explicit Rudder(Boat &_boat);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif //PPGSO_RUDDER_H
