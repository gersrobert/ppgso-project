#pragma once

#include <ppgso/ppgso.h>

#include "src/scene/object.h"

class BoatWheel;
class Mainsail;
class Foresail;
class WindVane;

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Asteroid objects in the scene
 */
class Boat final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float speed = 1.0;
    float acceleration = 0.0;
    float rotationSpeed = 0;
    float sailSheathe = 0.5f;
    float sailEffect = 0.0f;

    friend class BoatWheel;
    friend class Mainsail;
    friend class Foresail;
    friend class WindVane;

public:
    /*!
     * Create a new player
     */
    Boat(Scene &scene);

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

