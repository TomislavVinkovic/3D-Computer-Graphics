#include <iostream>
#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include "Shader.hpp"

//stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//GLM
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

static const unsigned int scr_width = 1920;
static const unsigned int scr_height = 1080;
static float FOV = 45.f;

//KAMERA
static glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
static glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
static glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);

//pitch, jaw i state misa
static float yaw = -90.f;
static float pitch = 0.f;
//po defaultu ovo ce se nalaziti na sredini ekrana
static float lastX = 400, lastY = 300;
static bool firstMouse = true;

//VARIJABLE ZA FIXANJE VELOCITYA(DELTA TIME)
static float deltaTime = 0.f;
static float lastFrame = 0.f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

int main() {

    //setup stbi
    stbi_set_flip_vertically_on_load(true);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "OpenGlTutorial", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    //ENABLANJE FEATUREA OPENGL-A
    glEnable(GL_DEPTH_TEST); //ukljuci z-buffer
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //--SETUP SHADERA--
    //-----------------------------
    Shader shader(
            "/home/tomislav/Desktop/faks/3d/projekt/OpenGLTutorial/shaders/texturedVertexShader.vert",
            "/home/tomislav/Desktop/faks/3d/projekt/OpenGLTutorial/shaders/texturedFragmentShader.frag"
            );
    shader.use();
    shader.setFloat("offset", 0.2f);

    //Za povrsinu na koju lijepimo teksturu cemo uzeti pravokutnik iz prvog poglavlja
    //vertexi pravokutnika
//    float vertices[] = {
//            // positions          // colors           // texture coords
//            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
//    };

    //vertexi KOCKE
    //vertexi sadrze teksturne koordinate i koordinate same kocke
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int indices[] = {
            0,1,3,
            1,2,3
    };

    unsigned int VBO, VAO;
    //unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO); //za pravokutnik nam dobro dode EBO

    //1. BINDAJ VERTEX ARRAY
    glBindVertexArray(VAO);

    //2. BINDAJ VERTEX BUFFER[E]
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //3. KONFIGURIRAJ VERTEX ATRIBUTE

    //atribut pozicije
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //atribut teksturne koordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //atribut boje
    /*
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
     */

    //SETUP TEKSTURA

    //generating a texture in OpenGL and binding it
    unsigned int texture1, texture2;
    //BOX TEXTURE
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("/home/tomislav/Desktop/faks/3d/projekt/OpenGLTutorial/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //HAPPY FACE TEXTURE
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("/home/tomislav/Desktop/faks/3d/projekt/OpenGLTutorial/textures/faceExpression.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //END OF TEXTURE SETUP

    //since we have more than one texture now, we must assign
    //them to our shader

    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    //main render loop
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //procesiranje inputa
        processInput(window);

        //render
        //---------

        //clearaj ekran
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //clearaj color i depth buffer svaki frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        //rotiranje kontejnera kroz vrijeme

        glBindVertexArray(VAO);

        //crtanje nase prve 3d scene!
        //-------------------------------

        //matrice transformacije
        //MODEL MATRIX(lokalno -> globalno)
        glm::mat4 model = glm::mat4(1.0f);

        //VIEW MATRIX
//        glm::mat4 view = glm::lookAt(
//            glm::vec3(0.f, 0.f, 3.f), //pozicija kamere
//            glm::vec3(0.f, 0.f, 0.f), //vektor gdje gleda kamera
//            glm::vec3(0.f, 1.f, 0.f) //vektor koji ide paralelno y osi 3D scene
//        );

          //kamera koja se rotira oko radijusa
          /*
          const float radius = 10.f;
          float camX = sin(glfwGetTime()) * radius;
          float camZ = cos(glfwGetTime()) * radius;
            glm::mat4 view = glm::lookAt(
            glm::vec3(camX, 0.f, camZ),
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f)
          );
           */

          //kamera koja se mice misem i tipkovnicom
          glm::mat4 view = glm::lookAt(
                  cameraPos, cameraPos + cameraFront, cameraUp
          );

        //PROJECTION MATRIX
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(FOV), (float)scr_width/(float)scr_height, 0.1f, 100.f);


        shader.setMatrix4fv("view", view);
        shader.setMatrix4fv("projection", projection);

        //crtanje jednog 2d kontejnera u 3d svijetu
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::vec3& cube = cubePositions[0];

        model = glm::mat4(1.f);
        //model = glm::translate(model, cube);
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.f));
        model = scale(model, glm::vec3(64.f, 16.f, 64.f));
        shader.setMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
//        //crtanje nase prve kocke
//        for(const auto& cube : cubePositions) {
//            model = glm::mat4(1.f);
//            model = glm::translate(model, cube);
//            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.f));
//            model = scale(model, glm::vec3(16.f, 16.f, 256.f));
//            shader.setMatrix4fv("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }

        //swapaj buffere i pollaj evente(keypressevi itd)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //pobrisi sve objekte koji vise nisu potrebni
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate(); //cleanaj svu memoriju
    return 0;
}


//ovdje ce se procesirati inputi s tipkovnice koje zelimo
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //kamera
    const float cameraSpeed = 10.f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //oduzimamo mu normalizirani right vektor
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //TOGGLE ZA WIREFRAME MOD
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(*polygonMode == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {

    if(firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    //obrnuto jer se y vrijednosti povecavaju od gore prema dolje
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    //restrikcija da se ne dogodi flip kamere
    if(pitch > 89.f) {
        pitch = 89.f;
    }
    if(pitch < -89.f) {
        pitch = -89.f;
    }

    //pitch/jaw kutevi kod kamere
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);

}
//mouse scroll
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
     FOV -= (float)yOffset;
     if(FOV < 1.f)
         FOV = 10.f;
     if(FOV > 65.f)
         FOV = 65.f;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width, height);
}