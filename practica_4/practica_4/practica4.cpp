/*
Importante: En este c�digo no est� declarado las variable uniform color, tampoco en todos los archivos que modificamos en clase la sesi�n pasada, por lo que 
si quieren usar la variable uniform para color debend e agregar todas las declaraciones de la sesi�n pasada en los archivos Shader.h, Shader.cpp, shader.vert.
El archivo Window.h y Window.cpp tiene los cambios para la rotaci�n alrededor del eje Y.
EL archivo Shader.h y el archivo Shader.cpp tienen las declaraciones de la variable uniform para la matriz de vista 
*/
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/random.hpp>
//clases para dar orden y limpieza al c�digo
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20);
void CreateObject()
{
	unsigned int indices[] = { 
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.5f,
		0.0f,-0.5f,0.5f,
		0.5f,-0.5f, 0.0f,
		0.0f,0.5f,0.0f
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
//ejercicio 1 para hacer en clase, el cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void iniciarEsfera(Sphere sp) {
	sp.init(); //inicializar esfera
	sp.load();
	sp.render();
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreateObject();
	CrearCubo();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::vec3 direccion = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));// ya est� indicado, pero todav�a no lo modificaremos
	//Loop mientras no se cierra la ventana
	float i, anchura;
	while (!mainWindow.getShouldClose())
	{
		view = glm::lookAt(glm::vec3(sin(glm::radians(mainWindow.getrotax()))*1, 0.0f, cos(glm::radians(mainWindow.getrotax()))*1), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));// ya est� indicado, pero todav�a no lo modificaremos
		direccion = glm::vec3(mainWindow.getmovx(), mainWindow.getmovy(), mainWindow.getmovz());
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		// Esfera superior
		glm::mat4 model(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::translate(model, direccion);
		model = glm::scale(model, glm::vec3(0.35f, 0.3f, 0.35f)); // Le da la forma de guevo
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f)); // Hace todo proporcionalmente mas chico
		color = glm::vec3(0.8915686274509804f, 0.8680392156862745f, 0.7150980392156863f);
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		iniciarEsfera(Sphere(1.0, 20, 20));
		i = 0.0f;
		anchura = 0.01f;
		while(i >= -1.1f){
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, i, -5.0f));
			model = glm::translate(model, direccion);
			model = glm::scale(model, glm::vec3(anchura, 0.05f, anchura));
			color = glm::vec3(0.9215686274509804f, 0.8980392156862745f, 0.7450980392156863f);
			model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			iniciarEsfera(Sphere(1.0, 20, 5));
			i -= 0.005f;
			anchura += 0.0013f;
		}

		// Disco que esta debajo de la esfera superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.30f, -5.0f));
		model = glm::translate(model, direccion);
		model = glm::scale(model, glm::vec3(0.3f, 0.065f, 0.3f));
		color = glm::vec3(0.9115686274509804f, 0.8880392156862745f, 0.7350980392156863f);
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		iniciarEsfera(Sphere(1.0, 20, 5));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -5.0f));
		model = glm::translate(model, direccion);
		model = glm::scale(model, glm::vec3(0.3f, 0.065f, 0.3f));
		color = glm::vec3(0.9115686274509804f, 0.8880392156862745f, 0.7350980392156863f);
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		iniciarEsfera(Sphere(1.0, 20, 5));

		// Base de la pieza
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, -5.0f));
		model = glm::translate(model, direccion);
		model = glm::scale(model, glm::vec3(0.5f, 0.15f, 0.5f));
		color = glm::vec3(0.9015686274509804f, 0.8780392156862745f, 0.7250980392156863f);
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		iniciarEsfera(Sphere(1.0, 20, 5));
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}