#ifndef PPGSO_TEXT_H
#define PPGSO_TEXT_H

#include <src/scene/vertex_object.h>
#include <src/scene/scene.h>

class Text final : public VertexObject {
private:
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    GLuint cbo = 0;
    std::vector<glm::vec4> colors;

    const std::string &text;

    struct Character {
        float u, v;
        float yOffset = 0;
        float width = 1;
    };
    std::map<char, Character> charConfig = {
            {'\'', {0.3f, 0.1f}},
            {' ', {0.0f, 0.0f}},
            {'!', {0.1f, 0.0f}},
            {'*', {0.0f, 0.1f}},
            {'+', {0.1f, 0.1f}},
            {'-', {0.3f, 0.1f}},
            {'/', {0.5f, 0.1f}},

            {'0', {0.6f, 0.1f}},
            {'1', {0.7f, 0.1f}},
            {'2', {0.8f, 0.1f}},
            {'3', {0.9f, 0.1f}},
            {'4', {0.0f, 0.2f}},
            {'5', {0.1f, 0.2f}},
            {'6', {0.2f, 0.2f}},
            {'7', {0.3f, 0.2f}},
            {'8', {0.4f, 0.2f}},
            {'9', {0.5f, 0.2f}},

            {'A', {0.3f, 0.3f}},
            {'B', {0.4f, 0.3f}},
            {'C', {0.5f, 0.3f}},
            {'D', {0.6f, 0.3f}},
            {'E', {0.7f, 0.3f}},
            {'F', {0.8f, 0.3f}},
            {'G', {0.9f, 0.3f}},
            {'H', {0.0f, 0.4f}},
            {'I', {0.1f, 0.4f}},
            {'J', {0.2f, 0.4f}},
            {'K', {0.3f, 0.4f}},
            {'L', {0.4f, 0.4f}},
            {'M', {0.5f, 0.4f}},
            {'N', {0.6f, 0.4f}},
            {'O', {0.7f, 0.4f}},
            {'P', {0.8f, 0.4f}},
            {'Q', {0.9f, 0.4f}},
            {'R', {0.0f, 0.5f}},
            {'S', {0.1f, 0.5f}},
            {'T', {0.2f, 0.5f}},
            {'U', {0.3f, 0.5f}},
            {'V', {0.4f, 0.5f}},
            {'W', {0.5f, 0.5f}},
            {'X', {0.6f, 0.5f}},
            {'Y', {0.7f, 0.5f}},
            {'Z', {0.8f, 0.5f}},

            {'a', {0.5f, 0.6f, -0.175f}},
            {'b', {0.6f, 0.6f}},
            {'c', {0.7f, 0.6f, -0.175f}},
            {'d', {0.8f, 0.6f}},
            {'e', {0.9f, 0.6f, -0.175f, 0.7f}},
            {'f', {0.0f, 0.7f}},
            {'g', {0.1f, 0.7f}},
            {'h', {0.2f, 0.7f}},
            {'i', {0.3f, 0.7f}},
            {'j', {0.4f, 0.7f}},
            {'k', {0.5f, 0.7f}},
            {'l', {0.6f, 0.7f, 0, 0.3f}},
            {'m', {0.7f, 0.7f, -0.175f}},
            {'n', {0.8f, 0.7f, -0.175f}},
            {'o', {0.9f, 0.7f, -0.175f, 0.8f}},
            {'p', {0.0f, 0.8f}},
            {'q', {0.1f, 0.8f}},
            {'r', {0.2f, 0.8f, -0.175f, 0.6f}},
            {'s', {0.3f, 0.8f, -0.175f}},
            {'t', {0.4f, 0.8f}},
            {'u', {0.5f, 0.8f, -0.175f}},
            {'v', {0.6f, 0.8f, -0.175f}},
            {'w', {0.7f, 0.8f, -0.175f}},
            {'x', {0.8f, 0.8f, -0.175f}},
            {'y', {0.9f, 0.8f, -0.175f}},
            {'z', {0.0f, 0.9f, -0.175f}},
    };

public:

    explicit Text(Scene &scene, const std::string &text, const glm::vec4 &color);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif //PPGSO_TEXT_H
