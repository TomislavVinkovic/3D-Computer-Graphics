#include "Renderer.h"

void Renderer::render(GLFWwindow *window, Camera &camera) {
    //std::cout << "Filled with fill color" << std::endl;
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    //glClearColor(0.f, 0.f, 0.f, 1.f); //when testing the skybox
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    cubeRenderer.render(camera);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::clearData() {
    cubeRenderer.deleteData();
}
