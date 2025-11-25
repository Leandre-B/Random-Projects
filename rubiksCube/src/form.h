#ifndef FORM_H
#define FORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Cube{
    public:
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        inline static const float vertices[180] = {
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
            0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f
        };
        unsigned int VBO, VAO;

        void make(){
            genBuffer();
            bindBuffer();

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        void glDelete(){
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

    private:

        void genBuffer(){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
        }

        void bindBuffer(){
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }

};

#endif

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};