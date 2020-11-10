#ifndef PPGSO_BEZIER_WAVES_H
#define PPGSO_BEZIER_WAVES_H

#include <ppgso/ppgso.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

#include "src/scene/object.h"

class BezierWaves final : public Object {
private:
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    struct face {
        GLuint v0, v1, v2;
    };
    std::vector<face> mesh;

    const static uint32_t CP_LEN = 8;
    std::vector<std::vector<glm::vec3>> controlPointsBase;

    GLuint vao = 0, vbo = 0, tbo = 0, ibo = 0, nbo = 0;
    glm::mat4 modelMatrix{1.0f};

public:
    /*!
     * Create new Sea background
     */
    BezierWaves();

    /*!
     * Update space background
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render space background
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};

#endif //PPGSO_BEZIER_WAVES_H
