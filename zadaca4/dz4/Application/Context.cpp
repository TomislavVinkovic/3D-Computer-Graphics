#include "Context.h"

Context::Context(const Camera& camera, unsigned int w, unsigned int h, const std::string& title) : Context(camera) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    scr_width = w;
    scr_height = h;

    this->window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

    if(window == NULL)
        reportCrash("Failed to create GLFW window");

    glfwMakeContextCurrent(window);
    initCallbacks();

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        reportCrash("Failed to init GLAD");

    //ENABLANJE INPUT FEATUREA OPENGL-A
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Context::adjustDeltaTime(){
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Context::reportCrash(const std::string&) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
}

void Context::handleKeyboardInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Movement::RIGHT, deltaTime);
}

void Context::handleKeyboardInterrupt(GLFWwindow *window, int key, int scancode, int action, int mods) {
    //TOGGLE ZA WIREFRAME MOD
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(*polygonMode == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else if(*polygonMode == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Context::handleMouseMovement(GLFWwindow *window, double xPos, double yPos) {
    float xPosf = static_cast<float>(xPos);
    float yPosf = static_cast<float>(yPos);
    if(firstMouse) {
        lastX = xPosf;
        lastY = yPosf;
        firstMouse = false;
    }

    float xOffset = xPosf - lastX;
    float yOffset = lastY - yPosf;

    lastX = xPosf;
    lastY = yPosf;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void Context::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,scr_width, scr_height);
}

void Context::initCallbacks() {
    glfwSetWindowUserPointer(window, this);

    //initanje svake funkcije na ovu instancu prozora
    auto framebuffer_func = [](GLFWwindow* w, int width, int height) {
        static_cast<Context*>(glfwGetWindowUserPointer(w))->framebuffer_size_callback(w, width, height);
    };

    auto interrupt_func = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<Context*>(glfwGetWindowUserPointer(w))->handleKeyboardInterrupt(
                w, key, scancode, action, mods
        );
    };

    auto mousemove_func = [](GLFWwindow* w, double xPos, double yPos) {
        static_cast<Context*>(glfwGetWindowUserPointer(w))->handleMouseMovement(
                w, xPos, yPos
        );
    };

    //assignanje tih funkcija na instancu prozora preko opengl-a
    glfwSetFramebufferSizeCallback(window, framebuffer_func);
    glfwSetKeyCallback(window, interrupt_func);
    glfwSetCursorPosCallback(window, mousemove_func);
}
