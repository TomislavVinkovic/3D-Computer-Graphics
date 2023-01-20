#pragma once
#include<vector>
#include<iostream>

class Model {
    protected:
        unsigned int VAO = 0;
        unsigned int EBO;
        unsigned int indices_count = 0;
        unsigned int VBO;

    public:
        virtual void setVertices(std::vector<float>) = 0;
        virtual void setIndices(std::vector<unsigned int>) = 0;

        void genVao() {
            glGenVertexArrays(1, &VAO);
        }

        void bindVAO() const {
            glBindVertexArray(VAO);
        }

        void deleteData() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            VAO = 0;
            EBO = 0; //PROMJENA : DODAO SAM INDEKS EBO-a tako da znam kako se zove
        }

        unsigned int getIndicesCount() {
            return indices_count;
        }

        //getters
        unsigned int getEBO() {
            return EBO;
        }
};