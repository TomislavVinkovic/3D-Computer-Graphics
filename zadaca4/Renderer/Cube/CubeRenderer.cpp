#include "CubeRenderer.h"

CubeRenderer::CubeRenderer(
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath,
    const std::string& lightVertexShaderPath,
    const std::string& lightFragmentShaderPath
    ) : shader(Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str())),
        lightShader(Shader(lightVertexShaderPath.c_str(), lightFragmentShaderPath.c_str())){
    //default cube
    std::vector<GLfloat> vertices = {
            //Back
            1, 0, 0, 0.f, 0.f, -1.f,
            0, 0, 0, 0.f, 0.f, -1.f,
            0, 1, 0, 0.f, 0.f, -1.f,
            1, 1, 0, 0.f, 0.f, -1.f,

            //Front
            0, 0, 1, 0.f, 0.f, 1.f,
            1, 0, 1, 0.f, 0.f, 1.f,
            1, 1, 1, 0.f, 0.f, 1.f,
            0, 1, 1, 0.f, 0.f, 1.f,

            //Right
            1, 0, 1, 1.f, 0.f, 0.f,
            1, 0, 0, 1.f, 0.f, 0.f,
            1, 1, 0, 1.f, 0.f, 0.f,
            1, 1, 1, 1.f, 0.f, 0.f,

            //Left
            0, 0, 0, -1.f, 0.f, 0.f,
            0, 0, 1, -1.f, 0.f, 0.f,
            0, 1, 1, -1.f, 0.f, 0.f,
            0, 1, 0, -1.f, 0.f, 0.f,

            //Top
            0, 1, 1, 0.f, 1.f, 0.f,
            1, 1, 1, 0.f, 1.f, 0.f,
            1, 1, 0, 0.f, 1.f, 0.f,
            0, 1, 0, 0.f, 1.f, 0.f,

            //Bottom
            0, 0, 0, 0.f, -1.f, 0.f,
            1, 0, 0, 0.f, -1.f, 0.f,
            1, 0, 1,  0.f, -1.f, 0.f,
            0, 0, 1, 0.f, -1.f, 0.f
    };
    std::vector<GLuint> indices = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
    };

    block.setVertices(vertices);
    block.setIndices(indices);

    light.setVertices(vertices);
    light.setIndices(indices);

    //add({0,0,3});
    lightPos = {1.5, 1.5, 0.5};
    blockPos = {0, 0, 1};
}

//void CubeRenderer::add(const glm::vec3 &pos) {
//    this->pos.push_back(pos);
//}

void CubeRenderer::render(Camera& camera) {
    shader.use();
    //texture.bindTexture();

    //draw the color block
    block.bindVAO();
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, blockPos);

    shader.setMat4("projection", camera.GetProjectionMatrix());
    shader.setMat4("model", model);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setVec3("objectColor", 0.1f, 0.8f, 0.9f);
    shader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.Position);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.getEBO());
    glDrawElements(GL_TRIANGLES, block.getIndicesCount(), GL_UNSIGNED_INT, 0);

    //draw the light
    lightShader.use();
    light.bindVAO();
    model = glm::mat4(1.f);
    model = glm::translate(model, lightPos);

    lightShader.setMat4("projection", camera.GetProjectionMatrix());
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", camera.GetViewMatrix());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light.getEBO());
    glDrawElements(GL_TRIANGLES, light.getIndicesCount(), GL_UNSIGNED_INT, 0);
}

void CubeRenderer::deleteData() {
    block.deleteData();
}
