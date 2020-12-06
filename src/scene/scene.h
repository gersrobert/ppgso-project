#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    virtual void update(float time) = 0;

    /*!
     * Render all objects in the scene
     */
    virtual void render() = 0;

    // All objects to be rendered in scene
    std::list<std::unique_ptr<Object>> objects;

    glm::mat4 guiProjection;
    std::list<std::unique_ptr<Object>> guiObjects;

    // Keyboard state
    std::map<int, int> keyboard;

    // Store cursor state
    struct {
        double x, y;
        bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
