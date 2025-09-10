
// Valeria Ram�rez Mart�nez 
// Pr�ctica 05
// Fecha de entrega: 09 / Septiembre / 2025
// No. Cuenta: 318063188

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

// Tama�o ventana
const GLint WIDTH = 1200, HEIGHT = 800;

// Para c�mara/movimiento
float movX = 0.0f, movY = 0.0f, movZ = -15.0f, rot = 0.0f;

// �ngulos de articulaciones
float hombro = 0.0f;
float codo = 0.0f;
float muneca = 0.0f;
float dedo1 = 0.0f, dedo2 = 0.0f, dedo3 = 0.0f, dedo4 = 0.0f, dedo5 = 0.0f;

// Funci�n auxiliar para dibujar un dedo 
void dibujarDedo(glm::mat4 base, float angulo, glm::vec3 offset, glm::vec3 color,
    GLuint modelLoc, GLuint uniformColor) {    
    glm::mat4 model = glm::translate(base, offset);
    model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 falange = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
    falange = glm::scale(falange, glm::vec3(1.0f, 0.3f, 0.25f));
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(falange));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    
    glm::mat4 distal = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
    distal = glm::rotate(distal, glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
    distal = glm::translate(distal, glm::vec3(0.5f, 0.0f, 0.0f));
    distal = glm::scale(distal, glm::vec3(1.0f, 0.3f, 0.25f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(distal));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 05   Valeria Ramirez", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    float vertices[] = {
        -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f,-0.5f,  0.5f, -0.5f,-0.5f,  0.5f,  0.5f,-0.5f,
         0.5f,  0.5f,-0.5f, -0.5f,  0.5f,-0.5f, -0.5f, -0.5f,-0.5f,
         0.5f, -0.5f, 0.5f,  0.5f, -0.5f,-0.5f,  0.5f,  0.5f,-0.5f,
         0.5f,  0.5f,-0.5f,  0.5f,  0.5f, 0.5f,  0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,-0.5f, -0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,
        -0.5f, -0.5f,-0.5f,  0.5f, -0.5f,-0.5f,  0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,-0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = glm::mat4(1);
        glm::mat4 modelTemp = glm::mat4(1.0f);
        glm::mat4 modelTemp2 = glm::mat4(1.0f);

        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint uniformColor = ourShader.uniformColor;

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        glBindVertexArray(VAO);

        // Brazo
        model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f));
        modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Antebrazo
        model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0, 0.0f));
        modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Palma
        model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(muneca), glm::vec3(0.0f, 1.0, 0.0f));
        modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dedos
        dibujarDedo(modelTemp2, dedo1, glm::vec3(0.25f, 0.35f, 0.4f), glm::vec3(0.0f, 1.0f, 1.0f), modelLoc, uniformColor);
        dibujarDedo(modelTemp2, dedo2, glm::vec3(0.25f, 0.0f, 0.4f), glm::vec3(1.0f, 0.0f, 1.0f), modelLoc, uniformColor);
        dibujarDedo(modelTemp2, dedo3, glm::vec3(0.25f, -0.35f, 0.4f), glm::vec3(1.0f, 1.0f, 0.0f), modelLoc, uniformColor);
        dibujarDedo(modelTemp2, dedo4, glm::vec3(0.25f, 0.2f, -0.4f), glm::vec3(0.5f, 0.2f, 1.0f), modelLoc, uniformColor);
        dibujarDedo(modelTemp2, dedo5, glm::vec3(0.25f, -0.2f, -0.4f), glm::vec3(0.2f, 1.0f, 0.5f), modelLoc, uniformColor);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movX += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movX -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += 0.18f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  rot -= 0.18f;

    // Hombro
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && hombro < 90.0f) hombro += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && hombro > -90.0f) hombro -= 0.5f;  
    
    // Codo
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && codo < 120.0f) codo += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && codo > 0.0f)   codo -= 0.5f;

    // Mu�eca
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && muneca < 60.0f) muneca += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && muneca > -60.0f) muneca -= 0.5f;


    // Dedos
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo1 < 90.0f) dedo1 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo1 > 0.0f)  dedo1 -= 0.5f;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && dedo2 < 90.0f) dedo2 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && dedo2 > 0.0f)  dedo2 -= 0.5f;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && dedo3 < 90.0f) dedo3 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && dedo3 > 0.0f)  dedo3 -= 0.5f;

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && dedo4 < 90.0f) dedo4 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && dedo4 > 0.0f)  dedo4 -= 0.5f;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && dedo5 < 90.0f) dedo5 += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && dedo5 > 0.0f)  dedo5 -= 0.5f;
}
