#ifndef PPGSO_SCREEN_OVERLAY_H
#define PPGSO_SCREEN_OVERLAY_H

#include <src/scene/vertex_object.h>
#include <src/scene/scene.h>
#include <src/objects/boat/boat.h>
#include "compass.h"

class ScreenOverlay final : public VertexObject {
private:
    static std::unique_ptr<ppgso::Shader> shader;

    glm::vec4 color = {0, 0.1, 0.21875, -1.0};

    Boat::Mode mode;

public:

    explicit ScreenOverlay(Scene &scene, Boat::Mode mode);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_SCREEN_OVERLAY_H
