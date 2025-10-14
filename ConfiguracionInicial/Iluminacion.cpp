// Valeria Ramírez Martínez
// Práctica 08
// Fecha de entrega: 14 / Octubre / 2025
// No. Cuenta: 318063188

// Std. Includes
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
GLuint createSphere(float radius, unsigned int rings, unsigned int sectors, GLuint& vbo, GLsizei& sphereVertexCount);

// Camera
Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Lights
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
glm::vec3 lightPos2(-2.0f, 1.0f, -1.5f);

float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool diaNoche = false;
float anguloLuz = 0.0f;

// Buffers
GLuint planeVAO, planeVBO;
GLuint cubeVAO, cubeVBO;
GLuint sphereVAO, sphereVBO;
GLsizei sphereVertexCount;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 08 Valeria Ramirez", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");

    // === Suelo ===
    GLfloat planeVertices[] = {
        // Posiciones          // Normales        // TexCoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    // VAO/VBO del suelo
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // === Cubo para las luces (sol y luna) ===
    float cubeVertices[] = {
        // posiciones
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    // VAO/VBO del cubo de luz
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);



    // === Modelos ===
    Model dog((char*)"Models/RedDog.obj");
    Model casa((char*)"Models/Hause 01.obj");


    // === Bucle principal ===
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // Movimiento día/noche (actualiza posiciones de luces si está activado)
        if (diaNoche)
        {
            float velocidad = 0.1f;  // movimiento más lento
            float radio = 5.0f;
            anguloLuz += deltaTime * velocidad;


            lightPos.x = radio * cos(anguloLuz);
            lightPos.y = radio * sin(anguloLuz);
            lightPos.z = 2.0f;

            lightPos2.x = -radio * cos(anguloLuz);
            lightPos2.y = -radio * sin(anguloLuz);
            lightPos2.z = -2.0f;
        }

        // ==== CÁLCULOS DE MATERIAL Y COLOR DE FONDO ====
        // intensidad entre 0.1 y 1.0 para evitar completa oscuridad
        float intensidadDia = glm::clamp((lightPos.y + 3.0f) / 6.0f, 0.1f, 1.0f);

        // Intensidades individuales de las fuentes
        float intensidadSol = glm::clamp(lightPos.y / 5.0f, 0.0f, 1.0f);  // más alto = más brillo
        float intensidadLuna = glm::clamp(lightPos2.y / 5.0f + 0.4f, 0.0f, 0.6f); // luna tenue

        // Material dinámico (ajusta rangos para que no quede todo gris)
        glm::vec3 matAmbient = glm::mix(glm::vec3(0.1f, 0.1f, 0.15f), glm::vec3(0.35f, 0.35f, 0.35f), intensidadDia);
        glm::vec3 matDiffuse = glm::mix(glm::vec3(0.15f, 0.2f, 0.25f), glm::vec3(0.9f, 0.85f, 0.75f), intensidadDia);
        glm::vec3 matSpecular = glm::mix(glm::vec3(0.2f, 0.2f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), intensidadDia);
        float matShininess = glm::mix(16.0f, 64.0f, intensidadDia);


        // Color de fondo (cielo) según hora
        glm::vec3 cieloDia(0.4f, 0.7f, 1.0f);
        glm::vec3 cieloNoche(0.03f, 0.04f, 0.08f);
        glm::vec3 colorFondo = glm::mix(cieloNoche, cieloDia, intensidadDia);
        glClearColor(colorFondo.r, colorFondo.g, colorFondo.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // === CONFIGURAR SHADER PRINCIPAL Y MATRICES ===
        shader.Use();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // enviar viewPos (necesario para specular)
        glm::vec3 camPos = camera.GetPosition();
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camPos.x, camPos.y, camPos.z);

        // === ENVIAR MATERIALES DINÁMICOS AL SHADER ===
        glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), matAmbient.x, matAmbient.y, matAmbient.z);
        glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), matDiffuse.x, matDiffuse.y, matDiffuse.z);
        glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), matSpecular.x, matSpecular.y, matSpecular.z);
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), matShininess);

        // === CONFIGURAR LUCES ==
        glm::vec3 solColor = glm::vec3(1.0f, 0.95f, 0.6f) * intensidadSol;
        glm::vec3 lunaColor = glm::vec3(0.5f, 0.6f, 1.0f) * intensidadLuna;

      
        // luz 1 
        glm::vec3 ambSol = glm::mix(glm::vec3(0.05f), glm::vec3(0.25f), intensidadDia); // ambiente del sol
        glUniform3f(glGetUniformLocation(shader.Program, "light1.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light1.ambient"), ambSol.x, ambSol.y, ambSol.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light1.diffuse"), solColor.x, solColor.y, solColor.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light1.specular"), 1.0f, 1.0f, 1.0f);

        // luz 2
        glUniform3f(glGetUniformLocation(shader.Program, "light2.position"), lightPos2.x, lightPos2.y, lightPos2.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light2.ambient"), 0.05f, 0.05f, 0.08f);
        glUniform3f(glGetUniformLocation(shader.Program, "light2.diffuse"), lunaColor.x, lunaColor.y, lunaColor.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light2.specular"), 0.6f, 0.6f, 0.9f);

        // === DIBUJAR SUELO ===
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // === DIBUJAR PERRO ===
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.0f, 5.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        // === DIBUJAR CASA ===
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(shader);


        // === DIBUJAR CUBOS DE LUZ (visual) ===
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Luz 1
        glm::mat4 modelLamp1 = glm::translate(glm::mat4(1.0f), lightPos);
        modelLamp1 = glm::scale(modelLamp1, glm::vec3(0.25f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp1));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), solColor.x, solColor.y, solColor.z);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Luz 2
        glm::mat4 modelLamp2 = glm::translate(glm::mat4(1.0f), lightPos2);
        modelLamp2 = glm::scale(modelLamp2, glm::vec3(0.25f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp2));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), lunaColor.x, lunaColor.y, lunaColor.z);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Intercambiar buffers
        glfwSwapBuffers(window);
    }


    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glfwTerminate();
    return 0;
}

//// === Generar esfera ===
GLuint createSphere(float radius, unsigned int rings, unsigned int sectors, GLuint& vbo, GLsizei& sphereVertexCount)
{
    std::vector<GLfloat> vertices;
    for (unsigned int r = 0; r <= rings; ++r)
    {
        for (unsigned int s = 0; s <= sectors; ++s)
        {
            float y = sin(-M_PI_2 + M_PI * r / rings);
            float x = cos(2 * M_PI * s / sectors) * sin(M_PI * r / rings);
            float z = sin(2 * M_PI * s / sectors) * sin(M_PI * r / rings);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);
        }
    }

    std::vector<GLuint> indices;
    for (unsigned int r = 0; r < rings; ++r)
        for (unsigned int s = 0; s < sectors; ++s)
        {
            indices.push_back(r * (sectors + 1) + s);
            indices.push_back((r + 1) * (sectors + 1) + s);
            indices.push_back((r + 1) * (sectors + 1) + (s + 1));
            indices.push_back(r * (sectors + 1) + s);
            indices.push_back((r + 1) * (sectors + 1) + (s + 1));
            indices.push_back(r * (sectors + 1) + (s + 1));
        }

    sphereVertexCount = indices.size();

    GLuint vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return vao;
}

// === Movimiento y entradas ===
void DoMovement()
{
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_N && action == GLFW_PRESS) diaNoche = !diaNoche;
    if (action == GLFW_PRESS) keys[key] = true;
    else if (action == GLFW_RELEASE) keys[key] = false;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}


