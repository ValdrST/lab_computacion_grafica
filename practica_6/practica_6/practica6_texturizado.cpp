/*
Semestre 2020-2
Pr�ctica 6 Texturizado
Usando librer�a stb_image.h 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <gtc/random.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include "Sphere.h"
#include"Window.h"
#include "Camera.h"
#include"Texture.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
float codo = 0.0f;

Texture T_ladrillos;
Texture T_asfalto;
Texture T_dado;
Camera camera;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader_t.vert";
static const char* fShader = "shaders/shader_t.frag";
Sphere sp = Sphere(1, 20, 20);

void CreateObject()
{
	unsigned int indices[] = { 
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.5f,	0.0f,	0.0f,
		0.0f,-0.5f,0.5f,	1.0f,	0.0f,
		0.5f,-0.5f, 0.0f,	1.0f,	1.0f,
		0.0f,0.5f,0.0f,		0.0f,	1.0f
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
		1, 4, 5,
		1, 5, 2,
		//left
		6, 0, 3,
		6, 3, 7,
		//bottom
		0, 1, 8, 
		1, 8, 9,
		// up
		2, 3, 10, 
		2, 10, 11,
		//back
		12, 13, 14, 
		12, 14, 15,
	};
GLfloat cubo_vertices[] = {
	// front
	//x		y		z		u		v
	-0.5f, -0.5f,  0.5f,	0.33f,	0.50f, // 0
	0.5f, -0.5f,  0.5f,		0.66f,	0.50f, // 1
	0.5f,  0.5f,  0.5f,		0.66f,	0.75f, // 2
	-0.5f,  0.5f,  0.5f,	0.33f,	0.75f, // 3
	//right
	0.5f, -0.5f, -0.5f,		0.99f,	0.50f, // 4
	0.5f,  0.5f, -0.5f,		0.99f,	0.75f, // 5
	//left
	-0.5f, -0.5f, -0.5f,	0.0f,	0.50f, // 6
	-0.5f,  0.5f, -0.5f,	0.0f,	0.75f, // 7
	//bottom
	-0.5f, -0.5f, -0.5f,	0.33f,	0.25f, // 8
	0.5f,  -0.5f, -0.5f,	0.66f,	0.25f, // 9
	// up
	-0.5f, 0.5f, -0.5f,		0.33f,	1.0f, // 10
	0.5f,  0.5f, -0.5f,		0.66f,	1.0f, // 11
	// back
	-0.5f, -0.5f,  -0.5f,	0.33f,	0.0f, // 12
	0.5f, -0.5f,  -0.5f,	0.66f,	0.0f, // 13
	0.5f,  0.5f,  -0.5f,	0.66f,	0.25f, // 14
	-0.5f,  0.5f,  -0.5f,	0.33f,	0.25f, // 15
	
};
int tamano_vertices = sizeof(cubo_vertices)/sizeof(cubo_vertices[0]);
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,tamano_vertices, 36);
meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main(){
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();
	CreateObject();
	CrearCubo();
	CreateShaders();
	camera = Camera(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.3f);
	//para crear la textura
	T_dado = Texture("Textures/dado.tga");
	T_dado.LoadTexture();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		//ejercicio 1:
		glm::mat4 model(1.0); 

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		model = glm::mat4(1.0);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		T_dado.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}