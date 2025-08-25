// Práctica 02
// Valeria Ramírez Martínez 
// Fecha de entrega: 20/ Agosto/ 2025
// No. Cuenta: 318063188


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 02 Valeria Ramirez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificaci�n de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// Primera línea
		-0.20f,  0.70f, 0.0f,   0.0f,0.0f,0.0f,
		-0.25f,  0.70f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.70f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.70f, 0.0f,   0.0f,0.0f,0.0f,

		// Segunda línea
		-0.40f,  0.63f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.63f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.63f, 0.0f,   0.0f,0.0f,0.0f,
		-0.25f,  0.63f, 0.0f,   1.0f,1.0f,1.0f,
		-0.20f,  0.63f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.63f, 0.0f,   0.0f,0.0f,0.0f,

		// Tercera línea
		-0.45f,  0.56f, 0.0f,   0.0f,0.0f,0.0f,
		-0.40f,  0.56f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.56f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.56f, 0.0f,   1.0f,1.0f,1.0f,
		-0.25f,  0.56f, 0.0f,   1.0f,1.0f,1.0f,
		-0.20f,  0.56f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.56f, 0.0f,   1.0f,1.0f,1.0f,
		-0.10f,  0.56f, 0.0f,   0.0f,0.0f,0.0f,

		// Cuarta línea
		-0.50f,  0.49f, 0.0f,   0.0f,0.0f,0.0f,
		-0.45f,  0.49f, 0.0f,   0.0f,0.0f,0.0f,
		-0.40f,  0.49f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.49f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.49f, 0.0f,   1.0f,1.0f,1.0f,
		-0.25f,  0.49f, 0.0f,   0.0f,0.0f,0.0f,
		-0.20f,  0.49f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.49f, 0.0f,   1.0f,1.0f,1.0f,
		-0.10f,  0.49f, 0.0f,   1.0f,1.0f,1.0f,
		-0.05f,  0.49f, 0.0f,   0.0f,0.0f,0.0f,

		// Quinta línea
		-0.50f,  0.42f, 0.0f,   0.0f,0.0f,0.0f,
		-0.45f,  0.42f, 0.0f,   0.0f,0.0f,0.0f,
		-0.40f,  0.42f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.42f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.42f, 0.0f,   1.0f,1.0f,1.0f,
		-0.25f,  0.42f, 0.0f,   1.0f,1.0f,1.0f,
		-0.20f,  0.42f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.42f, 0.0f,   1.0f,1.0f,1.0f,
		-0.10f,  0.42f, 0.0f,   1.0f,1.0f,1.0f,
		-0.05f,  0.42f, 0.0f,   0.0f,0.0f,0.0f,

		// Sexta línea
		-0.50f,  0.35f, 0.0f,   0.0f,0.0f,0.0f,
		-0.45f,  0.35f, 0.0f,   0.0f,0.0f,0.0f,
		-0.40f,  0.35f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.35f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.35f, 0.0f,   0.0f,0.0f,0.0f,
		-0.25f,  0.35f, 0.0f,   1.0f,1.0f,1.0f,
		-0.20f,  0.35f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.35f, 0.0f,   1.0f,1.0f,1.0f,
		-0.10f,  0.35f, 0.0f,   1.0f,1.0f,1.0f,
		-0.05f,  0.35f, 0.0f,   0.0f,0.0f,0.0f,

		// Séptima línea
		-0.50f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,
		-0.45f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,
		-0.40f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,
		-0.25f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,
		-0.20f,  0.28f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.28f, 0.0f,   1.0f,1.0f,1.0f,
		-0.10f,  0.28f, 0.0f,   1.0f,1.0f,1.0f,
		-0.05f,  0.28f, 0.0f,   0.0f,0.0f,0.0f,

		// Octava línea
		-0.45f,  0.21f, 0.0f,   0.0f,0.0f,0.0f,
		-0.40f,  0.21f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.21f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.21f, 0.0f,   1.0f,1.0f,1.0f,
		-0.25f,  0.21f, 0.0f,   0.0f,0.0f,0.0f,
		-0.20f,  0.21f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.21f, 0.0f,   1.0f,1.0f,1.0f,
		-0.10f,  0.21f, 0.0f,   0.0f,0.0f,0.0f,

		// Novena línea
		-0.40f,  0.14f, 0.0f,   0.0f,0.0f,0.0f,
		-0.35f,  0.14f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.14f, 0.0f,   0.0f,0.0f,0.0f,
		-0.25f,  0.14f, 0.0f,   0.0f,0.0f,0.0f,
		-0.20f,  0.14f, 0.0f,   1.0f,1.0f,1.0f,
		-0.15f,  0.14f, 0.0f,   0.0f,0.0f,0.0f,

		// Décima línea
		-0.35f,  0.07f, 0.0f,   0.0f,0.0f,0.0f,
		-0.30f,  0.07f, 0.0f,   0.0f,0.0f,0.0f,
		-0.25f,  0.07f, 0.0f,   0.0f,0.0f,0.0f,
		-0.20f,  0.07f, 0.0f,   0.0f,0.0f,0.0f,

	};


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)



	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);


		glPointSize(15);
		glDrawArrays(GL_POINTS, 0, 76);


		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}