/*
Semestre 2020-2
Pr�ctica 5 Modelado Jer�rquico
*/

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
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
float codo = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
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
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//CreateObject();
	CrearCubo();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//Loop mientras no se cierra la ventana
	sp.init();
	sp.load();

	while (!mainWindow.getShouldClose())
	{

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
		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

		//Creando un brazo formado por hombro, brazo,codo, antebrazo, mu�eca,palma, dedos.
		//hombro, codo y mu�eca s�lo son puntos de rotaci�n o articulaci�n, en este caso no dibujaremos esferas que los representen
		//Creando el hombro
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0); //matriz auxiliar
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.gethombro()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; //a model aux le pasamos rotacion y traslacion . Solo guarda informacion
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));//escalando la esfra
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//brazo
		//model = glm::mat4(1.0); //omitir estos resets y cambiarlos por matriz auxiliar que nos ayude a mantener jerarqu�a
		model = modelaux; //quitamos la escala con esto. Tiene rotacion y traslacion
		//model = glm::rotate(model, glm::radians(mainWindow.gethombro() * toRadians), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); //Y y Z estan a lam isma altura y profundidad
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[0]->RenderMesh();
		model = modelaux;
		//codo
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); //me traslado la mitad de mi prisma
		model = glm::rotate(model, glm::radians(mainWindow.getcodo()), glm::vec3(0.0f, 0.0f, 1.0f)); //se tiene que guardar para el antebrazo

		/*En caso de ser el codo una esfera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, -1.7f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		*/
		//antebrazo
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		modelaux = model; //para no guardar la escala de abajo
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//mu�eca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuneca()), glm::vec3(0.0f, 0.0f, 1.0f));

		/*En caso de ser la mu�eca una esfera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, -1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); */
		//palma
		color = glm::vec3(0.0f, 0.7f, 0.5f);
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		model = modelaux; //quitar escala de la palma
		glm::mat4 posicionpalma(1.0);
		posicionpalma = model; //matiz al centro de la palma que no se mueve


		//nudillo dedo indice
		color = glm::vec3(0.3f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.15f, 0.15f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice()), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice_z()), glm::vec3(0.0f, 0.0f, 1.0f));

		//falange dedo �ndice
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falamge falangina dedo indice
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo �ndice       
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.09f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta dedo indice

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_indice()), glm::vec3(0.0f, -1.0f, 0.0f));


		//falangeta dedo �ndice
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.08f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		//completar la mano y agregar colores diferentes a cada dedo.
		color = glm::vec3(0.3f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux = posicionpalma;

		//nudillo dedo medio
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_medio()), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_medio_z()), glm::vec3(0.0f, 0.0f, 1.0f));

		//falange dedo medio
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.12f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falamge falangina dedo medio
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_medio()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo medio       
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.11f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta dedo medio

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_medio()), glm::vec3(0.0f, -1.0f, 0.0f));


		//falangeta dedo medio
		model = glm::translate(model, glm::vec3(0.17f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Dedo anular
		color = glm::vec3(0.0f, 0.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux = posicionpalma;

		//nudillo dedo anular
		model = glm::translate(model, glm::vec3(0.15f, -0.12f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_anular()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falange dedo anular
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.28f, 0.07f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falamge falangina dedo anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_anular()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo anular       
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.37f, 0.06f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta dedo anular

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_anular()), glm::vec3(0.0f, -1.0f, 0.0f));


		//falangeta dedo anular
		model = glm::translate(model, glm::vec3(0.17f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.05f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		
		// Dedo peque�o
		color = glm::vec3(0.5f, 0.7f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux = posicionpalma;

		//nudillo dedo peque�o
		model = glm::translate(model, glm::vec3(0.15f, -0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_menique()), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_menique_z()), glm::vec3(0.0f, 0.0f, 1.0f));

		//falange dedo peque�o
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.04f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falange falangina dedo peque�o
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_menique()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo peque�o  
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.35f, 0.035f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta dedo peque�o

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_menique()), glm::vec3(0.0f, -1.0f, 0.0f));


		//falangeta dedo peque�o
		model = glm::translate(model, glm::vec3(0.17f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.04f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Dedo pulgar
		color = glm::vec3(0.8f, 0.3f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux = posicionpalma;

		//nudillo dedo pulgar
		model = glm::translate(model, glm::vec3(0.1f, 0.12f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_pulgar()), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_pulgar_z()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_pulgar_x()), glm::vec3(1.0f, 0.0f, 0.0f));

		/*//falange dedo pulgar
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.04f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falamge falangina dedo pulgar
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(0.0f, -1.0f, 0.0f));
		*/
		//falangina dedo pulgar 
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.35f, 0.035f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta dedo pulgar

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_pulgar()), glm::vec3(0.0f, -1.0f, 0.0f));


		//falangeta dedo pulgar
		model = glm::translate(model, glm::vec3(0.17f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.04f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}