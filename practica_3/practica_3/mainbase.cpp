#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0f; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
void CrearTriangulo()
{
	unsigned int indices[] = { 
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,
		0.0f,-0.5f,0.5f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f,

	};
	Mesh *triangulo = new Mesh();
	int num_of_indices = sizeof(indices) / sizeof(indices[0]);
	int num_of_vertices = sizeof(vertices) / sizeof(vertices[0]);
	triangulo->CreateMesh(vertices, indices, num_of_indices, num_of_vertices);
	meshList.push_back(triangulo);
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

void generarTrianguloRubik(glm::mat4 model, glm::mat4 projection, glm::vec3 color, GLuint uniformModel, GLuint uniformProjection, GLuint uniformColor) {
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
	model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(0.0f, 0.0f, 1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(1.0f, 1.0f, 0.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 1.0f, -1.0f));
	model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(1.0f, 1.0f, 0.0f);
	model = glm::translate(model, glm::vec3(-0.5f, -1.0f, -1.0f));
	model = glm::rotate(model, 0 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	model = glm::translate(model, glm::vec3(-1.5f, -1.0f, -1.0f));
	model = glm::rotate(model, 0 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0);
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	model = glm::translate(model, glm::vec3(0.5f, -1.0f, -1.0f));
	model = glm::rotate(model, 0 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	meshList[0]->RenderMesh();
}
float valorX, valorY, valorZ;
float persp = 45.0f;
float rotX = 0.0f;
float rotY = 0.0f;
float rotZ = 0.0f;
int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CrearTriangulo();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformColor = 0;
	GLuint uniformView = 0;
	//glm::mat4 projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 10.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//projection = glm::perspective(glm::radians(persp), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.5f, 100.0f);
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformColor = shaderList[0].GetColorLocation();
		glm::mat4 model(1.0); 
		//model = glm::mat4(1.0);

		//////////////// ROTACIÓN //////////////////
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		////////////////  ESCALA ////////////////

		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		///////////////////// T+R////////////////
		//model = glm::translate(model, glm::vec3(valorX, 0.0f, 0.0f));
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, rotZ));
		
		/////////////R+T//////////
		/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
		*/
		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		// generarTrianguloRubik(model, projection, color, uniformModel, uniformProjection, uniformColor);
		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		//model = glm::rotate(model, 70 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::translate(model, glm::vec3(valorX, valorY-1,-10.0f));
		//model = glm::rotate(model, 70 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (180+70) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}