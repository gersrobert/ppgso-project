#ifndef PPGSO_VERTEX_OBJECT_H
#define PPGSO_VERTEX_OBJECT_H

#include <dependencies/include/GL/glew.h>
#include <vector>
#include <dependencies/include/glm/vec3.hpp>
#include <dependencies/include/glm/detail/type_mat.hpp>
#include <shader.h>
#include <texture.h>
#include "object.h"

class VertexObject : public Object {
protected:

    struct face {
        GLuint v0, v1, v2;
    };

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<face> faces;

    GLuint vao = 0, vbo = 0, tbo = 0, ibo = 0, nbo = 0;
    glm::mat4 modelMatrix{1.0f};

    VertexObject() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &tbo);
        glGenBuffers(1, &ibo);
        glGenBuffers(1, &nbo);
    }

    ~VertexObject() override {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &tbo);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &nbo);
    }

    void setVertexBuffer(std::unique_ptr<ppgso::Shader>& shader) {
        // Copy positions to gpu
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        // Set vertex program inputs
        auto position_attrib = shader->getAttribLocation("Position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    void setTextureBuffer(std::unique_ptr<ppgso::Shader>& shader) {
        // Copy texture positions to gpu
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

        // Set vertex program inputs
        auto texCoord_attrib = shader->getAttribLocation("TexCoord");
        glEnableVertexAttribArray(texCoord_attrib);
        glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    void setNormalBuffer(std::unique_ptr<ppgso::Shader>& shader) {
        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(),
                     GL_STATIC_DRAW);

        auto normalCoord_attrib = shader->getAttribLocation("Normal");
        glEnableVertexAttribArray(normalCoord_attrib);
        glVertexAttribPointer(normalCoord_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    void setIndexBuffer() {
        // Copy indices to gpu
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(face), faces.data(), GL_STATIC_DRAW);
    }
};

#endif //PPGSO_VERTEX_OBJECT_H
