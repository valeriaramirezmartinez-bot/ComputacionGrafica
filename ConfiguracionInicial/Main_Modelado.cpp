// Valeria Ramírez Martínez
// Práctica 04
// Fecha de entrega: 02 / Septiembre / 2025
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

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -8.0f;
float rot = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 04  Valeria Ramirez", nullptr, nullptr);

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
        // Posición           // Color (no usado, pero necesario para layout)
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 0.0f,

         0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,-0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,-0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,-0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,-0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 1.0f,

        -0.5f,  0.5f,-0.5f,   1.0f, 0.2f, 0.5f,
         0.5f,  0.5f,-0.5f,   1.0f, 0.2f, 0.5f,
         0.5f,  0.5f, 0.5f,   1.0f, 0.2f, 0.5f,
         0.5f,  0.5f, 0.5f,   1.0f, 0.2f, 0.5f,
        -0.5f,  0.5f, 0.5f,   1.0f, 0.2f, 0.5f,
        -0.5f,  0.5f,-0.5f,   1.0f, 0.2f, 0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color dummy
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint colorLoc = glGetUniformLocation(ourShader.Program, "uniColor");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        glm::mat4 model;

        // ----- Cuerpo -----
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.3f, 0.0f));  // Colocamos el cubo en la posición del cuerpo
        model = glm::scale(model, glm::vec3(1.5f, 0.6f, 2.0f));   // Ensanchamos el cubo, para tener la forma rectangular
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));   // 
        glUniform3f(colorLoc, 0.6f, 1.0f, 0.6f);   // Asignamos el color verde 
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ----- Cabeza -----
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, 1.6f));  // Posiciona el cubo delante del cuerpo
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));  // Modificamos la escala para que sea más pequeño que el cuerpo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(colorLoc, 0.5f, 1.0f, 0.5f);   // Asignamos un color verde más claro
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ----- Ojo derecho -----
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.1f, 2.0f));   // Se crea un cubo a la derecha
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.1f));    // Nos aseguramos que sea pequeño para que parezca ojo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);   // Asignamos el color negro
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ----- Ojo izquierdo -----
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 0.1f, 2.0f));   // Se crea un cubo a la izquierda
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.1f));   // Nos aseguramos que sea pequeño para que parezca ojo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);    // Asignamos el color negro
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ----- Patas -----
        float patas[4][3] = { {1.0f, -0.8f, 1.0f}, {-1.0f, -0.8f, 1.0f}, {1.0f, -0.8f, -1.0f}, {-1.0f, -0.8f, -1.0f} };  // Declaramos las 4 posiciones para las patas
        for (int i = 0; i < 4; i++) {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(patas[i][0], patas[i][1], patas[i][2]));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));   // Declaramos el tamaño de las patas
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform3f(colorLoc, 0.3f, 0.9f, 0.3f);   // Asignamos el color verde 
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ----- Caparazón naranja -----
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 0.8f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(colorLoc, 1.0f, 0.5f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ----- Caparazón marrón -----
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(colorLoc, 0.5f, 0.2f, 0.1f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot -= 0.4f;
}

