// Valeria Ramírez Martínez 
// Práctica 07
// Fecha de entrega: 06 / Octubre / 2025
// No. Cuenta: 318063188

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// stb_image para cargar imágenes
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Includes personalizados
#include "Shader.h"
#include "Camera.h"

// Prototipos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Dimensiones
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámara
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Función principal
int main()
{
    // Inicializa GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 07  Valeria Ramirez", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Error al crear ventana GLFW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error al inicializar GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Compilar shader
    Shader shader("Shader/lamp.vs", "Shader/lamp.frag");

    // Vértices del cubo con coordenadas UV del mapa tipo dado
    GLfloat cubeVertices[] = {
        // Cara frontal (1) → u: -0.005 a 0.1716
        -0.5f, -0.5f,  0.5f,   1,1,1,   -0.005f,  0.0f,
         0.5f, -0.5f,  0.5f,   1,1,1,    0.1716f,  0.0f,
         0.5f,  0.5f,  0.5f,   1,1,1,    0.1716f,  1.0f,
        -0.5f,  0.5f,  0.5f,   1,1,1,   -0.005f,  1.0f,

        // Cara derecha (2) → u: 0.1616 a 0.3383
         0.5f, -0.5f, -0.5f,   1,1,1,    0.1616f,  0.0f,
         0.5f, -0.5f,  0.5f,   1,1,1,    0.3383f,  0.0f,
         0.5f,  0.5f,  0.5f,   1,1,1,    0.3383f,  1.0f,
         0.5f,  0.5f, -0.5f,   1,1,1,    0.1616f,  1.0f,

         // Cara inferior (3) → u: 0.3283 a 0.505
         -0.5f, -0.5f, -0.5f,   1,1,1,    0.3283f,  0.0f,
          0.5f, -0.5f, -0.5f,   1,1,1,    0.505f,   0.0f,
          0.5f, -0.5f,  0.5f,   1,1,1,    0.505f,   1.0f,
         -0.5f, -0.5f,  0.5f,   1,1,1,    0.3283f,  1.0f,

         // Cara superior (4) → u: 0.495 a 0.6716
         -0.5f,  0.5f, -0.5f,   1,1,1,    0.495f,   0.0f,
          0.5f,  0.5f, -0.5f,   1,1,1,    0.6716f,  0.0f,
          0.5f,  0.5f,  0.5f,   1,1,1,    0.6716f,  1.0f,
         -0.5f,  0.5f,  0.5f,   1,1,1,    0.495f,   1.0f,

         // Cara izquierda (5) → u: 0.6616 a 0.8383
         -0.5f, -0.5f, -0.5f,   1,1,1,    0.6616f,  0.0f,
         -0.5f, -0.5f,  0.5f,   1,1,1,    0.8383f,  0.0f,
         -0.5f,  0.5f,  0.5f,   1,1,1,    0.8383f,  1.0f,
         -0.5f,  0.5f, -0.5f,   1,1,1,    0.6616f,  1.0f,

         // Cara trasera (6) → u: 0.8283 a 1.0 (clamp)
         -0.5f, -0.5f, -0.5f,   1,1,1,    0.8283f,  0.0f,
          0.5f, -0.5f, -0.5f,   1,1,1,    1.0f,     0.0f,
          0.5f,  0.5f, -0.5f,   1,1,1,    1.0f,     1.0f,
         -0.5f,  0.5f, -0.5f,   1,1,1,    0.8283f,  1.0f,
    };


    GLuint cubeIndices[] = {
        0,1,2, 2,3,0,       // frontal (1)
        4,5,6, 6,7,4,       // trasera (6)
        8,9,10,10,11,8,     // izquierda (5)
       12,13,14,14,15,12,   // derecha (2)
       16,17,18,18,19,16,   // superior (4)
       20,21,22,22,23,20    // inferior (3)
    };


    // Buffers
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Atributos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Cargar textura
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("images/dado.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fallo al cargar textura." << std::endl;
    }
    stbi_image_free(data);

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model(1);

        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc = glGetUniformLocation(shader.Program, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}

// Movimiento cámara
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
