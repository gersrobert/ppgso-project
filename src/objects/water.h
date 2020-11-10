#ifndef PPGSO_WATER_H
#define PPGSO_WATER_H

#include <bits/unique_ptr.h>
#include <shader.h>
#include <src/scene/scene.h>

class Water final : public Object {
private:
    // Static resources (Shared between instances)
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    struct face {
        GLuint v0, v1, v2;
    };

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<face> faces;

    GLuint vao = 0, vbo = 0, tbo = 0, ibo = 0, nbo = 0;
    glm::mat4 modelMatrix{1.0f};

public:

    explicit Water(Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};

#endif //PPGSO_WATER_H
