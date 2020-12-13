#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

class Scene;

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
    glm::vec3 position{0, 0, 0};
    glm::vec3 positionOffset{0, 0, 0};
    glm::vec3 offset{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};

    float distance = 0;

    glm::vec3 up{0, 1, 0};
    glm::vec3 back{0, 0, -1};

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    enum Mode {THIRD_PERSON, CINEMATIC};
    Mode cameraMode;

    /*!
     * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
     * @param fov - Field of view in degrees
     * @param ratio - Viewport screen ratio (usually width/height of the render window)
     * @param near - Distance to the near frustum plane
     * @param far - Distance to the far frustum plane
     */
    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

    /*!
     * Update Camera viewMatrix based on up, position and back vectors
     */
    void update(Scene &scene, float time);

    /*!
     * Get direction vector in world coordinates through camera projection plane
     * @param u - camera projection plane horizontal coordinate [-1,1]
     * @param v - camera projection plane vertical coordinate [-1,1]
     * @return Normalized vector from camera position to position on the camera projection plane
     */
    glm::vec3 cast(double u, double v);

    void moveTo(const glm::vec3 &pos, const glm::vec3 &rot);

    glm::vec3 getTotalPosition() const;

private:
    void moveThirdPerson(Scene &scene, float time);

    void initialize(Scene &scene);
    bool initialized = false;
};

