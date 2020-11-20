#ifndef PPGSO_FORESAIL_H
#define PPGSO_FORESAIL_H

#include <src/scene/object.h>
#include <src/scene/scene.h>
#include "boat.h"

class Foresail final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    Boat &boat;
    glm::vec3 offset;
    float spin = 0;

public:
    /*!
     * Create a new player
     */
    Foresail(Scene &scene, Boat &boat);

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

};

#endif //PPGSO_FORESAIL_H
