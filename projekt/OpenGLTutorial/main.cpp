#include <iostream>
#include<cmath>
#include "glad/glad.h"
#include<GLFW/glfw3.h>

//ovdje vertex shader zapovijeda fragment shaderu
//koju boju mora koristiti ako je ovaj prima na inputu
//s istim tipom i imenom
static const char *vertexShaderSource =
     "#version 330 core\n"
     "layout (location = 0) in vec3 aPos;\n"
     "layout (location = 1) in vec3 aColor;"
     "out vec3 ourColor;\n"
     "void main()\n"
     "{\n"
     "gl_Position = vec4(aPos, 1.0);\n"
     "ourColor = aColor;\n"
     "}\0";

static const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main()\n"
        "{FragColor = vec4(ourColor, 1.0f);}\0";

static const unsigned int scr_width = 800;
static const unsigned int scr_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    //--SETUP SHADERA--
    //-----------------------------

    //--flags
    int success;
    char infoLog[512];

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //provjera je li se shader dobro kompajlirao
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) { //ERROR LOGGING
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //-------------------------------
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //provjera je li se shader dobro kompajlirao
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //LINKANJE SHADERA
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //provjera je li se program dobro linkao
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) { //ERROR LOGGING
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //SETUP ZA CRTANJE TROKUTA
    //trokuti s vise atributa
    //u ovom slucaju, boje!
    float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //glGenBuffers(1, &EBO);

    //1. PRVO BINDAJ VERTEX ARRAY
    glBindVertexArray(VAO);

    //2. ONDA BINDAJ VERTEX BUFFER[E]
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //3. I NA KRAJU KONFIGURIRAJ VERTEX ATRIBUTE
    //stride je sad drukciji jer svaki vertex ima 2 atributa
    //svaki je float i svaki je velicine 3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //dodatak za boje
    //ovdje moramo specificirati drukciji offset
    //razlog tomu je sto nam je pozicija prvi atribut, a boja drugi
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //main render loop
    while(!glfwWindowShouldClose(window)) {
        //procesiranje inputa
        processInput(window);

        //render
        //---------

        //clearaj ekran
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //nacrtaj trokut s prvim shaderom
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //swapaj buffere i pollaj evente(keypressevi itd)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //pobrisi sve objekte koji vise nisu potrebni
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); //cleanaj svu memoriju
    return 0;
}


//ovdje ce se procesirati inputi s tipkovnice koje zelimo
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //TOGGLE ZA WIREFRAME MOD


}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(*polygonMode == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width, height);
}