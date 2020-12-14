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

    enum Mode {
        GAME, COLLISION, END
    };

private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Texture> texture;

    float speed = 1.0;
    float rotationSpeed = 0;
    float sailSheathe = 0.5f;

    friend class BoatWheel;
    friend class Mainsail;
    friend class Foresail;
    friend class WindVane;

    float calculateSailEffect(Scene &scene, float dt);
    static float calculateSpeed(float currentSpeed, float sailForce);
    void checkCollisions(Scene &scene, float dt);

    Mode mode = GAME;

    bool updateGame(Scene &scene, float dt);
    bool updateCollision(Scene &scene, float dt);
};

