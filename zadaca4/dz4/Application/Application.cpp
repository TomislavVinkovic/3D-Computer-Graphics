#include "Application.h"

//zasad cu hardkodirat atribute za renderer
Application::Application(unsigned int w, unsigned int h, const std::string &name, const Camera& camera)
    : context(Context(camera, w, h, name)), renderer(
        w, h,
        "/home/tomislav/Desktop/faks/3d/zadaca4/dz4/shaders/normalVertexShader.vert",
        "/home/tomislav/Desktop/faks/3d/zadaca4/dz4/shaders/normalFragmentShader.frag",
        "/home/tomislav/Desktop/faks/3d/zadaca4/dz4/shaders/lightVertexShader.vert",
        "/home/tomislav/Desktop/faks/3d/zadaca4/dz4/shaders/lightFragmentShader.frag"
    ) {}

void Application::runLoop() {
    context.camera.ProcessMouseMovement(0.1, 0);
    while(!glfwWindowShouldClose(context.window)) {
        context.adjustDeltaTime();
        context.handleKeyboardInput(context.window); //kontinuriani input
        renderer.render(context.window, context.camera);
    }
    renderer.clearData();
    glfwTerminate();
}
