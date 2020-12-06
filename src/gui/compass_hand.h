#ifndef PPGSO_COMPASS_HAND_H
#define PPGSO_COMPASS_HAND_H

#include <src/scene/vertex_object.h>
#include <src/scene/scene.h>

class CompassHand final : public VertexObject {
private:
    static std::unique_ptr<ppgso::Shader> shader;

    GLuint cbo;
    std::vector<glm::vec4> colors;

public:

    explicit CompassHand(Scene &scene);
    ~CompassHand() override;

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_COMPASS_HAND_H
