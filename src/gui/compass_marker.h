#ifndef PPGSO_COMPASS_MARKER_H
#define PPGSO_COMPASS_MARKER_H

#include <src/scene/vertex_object.h>
#include <src/scene/scene.h>

class CompassMarker final : public VertexObject {
private:
    static std::unique_ptr<ppgso::Shader> shader;

    GLuint cbo;
    std::vector<glm::vec4> colors;

public:

    explicit CompassMarker(Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_COMPASS_MARKER_H
