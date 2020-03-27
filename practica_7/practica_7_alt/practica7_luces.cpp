/*
Semestre 2020-2
Pr�ctica 7 Iluminaci�n 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture dado10Texture;
Texture skyTexture;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = (-1*vec.x); vertices[nOffset + 1] = (-1*vec.y); vertices[nOffset + 2] = (-1*vec.z);
	}
}

void CreateObjects() 
{
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};
	int tamano_vertices = sizeof(vertices)/sizeof(vertices[0]);
	int tamano_indices = sizeof(indices)/sizeof(indices[0]);
	calcAverageNormals(indices, tamano_indices, vertices, tamano_vertices, 8, 5);
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-1.0f, 0.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, -1.0f,	1.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,	0.0f, 1.0f,	0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,		1.0f, 1.0f,	0.0f, 0.0f, 0.0f
	};
	calcAverageNormals(floorIndices, 6, floorVertices, 32, 8, 5);
	/*int z = 8;
	for(int x=0;x<4;x++){
		printf("%f, %f, %f\n",floorVertices[x*z + 5],floorVertices[x*z +6],floorVertices[x*z + 7]);
		printf("|%d %d %d|\n",(x*z) + 5,(x*z) + 6,(x*z) + 7);
	}*/
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales en el eje X la normal va inversa, en el caso del eje z igual, en el caso del eje y va directo
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	1.0f,  	0.0f,		0.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    1.0f,  	0.0f,		0.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    1.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	1.0f,  	0.0f,		0.0f,	0.0f,	0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	1.0f,  	0.0f,		0.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	1.0f,  	0.0f,		0.0f,	0.0f,	0.0f,//y negativo
		0.5f,  -0.5f,  0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	1.0f,  	0.0f,		0.0f,	0.0f,	0.0f, //y positivo
		 0.5f,  0.5f,  0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,

	};
	int tamano_vertices = sizeof(cubo_vertices)/sizeof(cubo_vertices[0]);
	calcAverageNormals(cubo_indices, 36, cubo_vertices, tamano_vertices, 8, 5);
	/*int z = 8;
	for(int x=0;x<24;x++){
		printf("%f, %f, %f\n",cubo_vertices[x*z + 5],cubo_vertices[x*z +6],cubo_vertices[x*z + 7]);
		printf("|%d %d %d|\n",(x*z) + 5,(x*z) + 6,(x*z) + 7);
	}*/
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, tamano_vertices, 36);
	meshList.push_back(cubo);

}

void CrearDado10(){
	unsigned int dado_indices[] = {
		// Arriba
		0, 1, 2,
		2, 3, 0,
		0, 3, 4,
		4, 5, 0,
		0, 5, 6,
		6, 7, 0,
		0, 7, 8,
		8, 9, 0,
		0, 9, 10,
		10, 11, 0,
		// Abajo
		23, 12, 13,
		13, 14, 23,
		23, 14, 15,
		14, 15, 23,
		23, 15, 16,
		16, 17, 23,
		23, 17, 18,
		18, 19, 23,
		23, 19, 20,
		20, 21, 23,
		23, 21, 22
	};
GLfloat dado_vertices[] = {
	// front
	//x		y		z		u		v
	0.0f, 0.0f, 0.5f, 0.637f, 0.7402f, 0.0f,	0.0f,	0.0f, // superior
	0.5f, 0.0f, 0.05f, 0.9942f, 0.8781f, 0.0f,	0.0f,	0.0f,// 1
	0.4f, 0.29f, -0.05f, 0.8886f, 0.981f, 0.0f, 0.0f,	0.0f,// 2
	0.15f, 0.48f, 0.05f, 0.6776f, 0.9782f, 0.0f, 0.0f,	0.0f,// 3
	-0.15f, 0.48f, -0.05f, 0.4639f, 1.0f, 0.0f,	0.0f,	0.0f,// 4
	-0.4f, 0.29f, 0.05f, 0.3259f, 0.9133f, 0.0f, 0.0f,	0.0f,// 5
	-0.5f, 0.0f, -0.05f, 0.1771f, 0.8186f, 0.0f, 0.0f,	0.0f,// 6
	-0.4f, -0.29f, 0.05f, 0.2176f, 0.6969f, 0.0f, 0.0f,	0.0f,// 7
	-0.15f, -0.48f, -0.05f,0.2501f, 0.5751f, 0.0f, 0.0f,	0.0f,// 8
	0.15f, -0.48f, 0.05f, 0.4449f, 0.5345f, 0.0f, 0.0f,	0.0f,// 9
	0.4f, -0.29f, -0.05f, 0.6343f, 0.4669f, 0.0f, 0.0f,	0.0f,// 10
	0.5f, 0.0f, 0.05f, 0.8183f, 0.5264f, 0.0f, 0.0f,	0.0f,// 11
	0.4f, -0.29f, -0.05f, 0.6343f, 0.4669f, 0.0f, 0.0f,	0.0f,// 12 10
	0.15f, -0.48f, 0.05f, 0.4449f, 0.5345f, 0.0f, 0.0f,	0.0f,// 13 9
	-0.15f, -0.48f, -0.05f, 0.2609f, 0.4669f, 0.0f, 0.0f,	0.0f,// 14 8
	-0.4f, -0.29f, 0.05f, 0.0661f, 0.4155f,0.0f, 0.0f,	0.0f, // 15 7
	-0.5f, 0.0f, -0.05f, 0.0391f, 0.291f,0.0f, 0.0f,	0.0f, // 16 6 
	-0.4f, 0.29f, 0.05f, 0.0012f, 0.1666f, 0.0f, 0.0f,	0.0f,// 17 5
	-0.15f, 0.48f, -0.05f, 0.1365f, 0.0854f, 0.0f, 0.0f,	0.0f,// 18 4
	0.15f, 0.48f, 0.05f, 0.2745f, 0.0042f, 0.0f, 0.0f,	0.0f,// 19 3
	0.4f, 0.29f, -0.05f, 0.4882f, 0.015f,0.0f, 0.0f,	0.0f, // 20 2
	0.5f, 0.0f, 0.05f, 0.7019f, 0.0259f, 0.0f, 0.0f,	0.0f,// 21 1
	0.4f, -0.29f, -0.05f, 0.8102f, 0.1233f, 0.0f, 0.0f,	0.0f,// 22 12
	0.0f, 0.0f, -0.5f,0.4449f, 0.2504f, 0.0f, 0.0f,	0.0f,// Inferior
};

int tamano_vertices = sizeof(dado_vertices)/sizeof(dado_vertices[0]);
int tamano_indices = sizeof(dado_indices)/sizeof(dado_indices[0]);
calcAverageNormals(dado_indices, tamano_indices, dado_vertices, tamano_vertices, 8, 5);
Mesh *cubo = new Mesh();
cubo->CreateMesh(dado_vertices, dado_indices,tamano_vertices, tamano_indices);
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
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();
	CrearDado10();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTexture();
	dado10Texture = Texture("Textures/dado_10.tga");
	dado10Texture.LoadTexture();
	
	skyTexture = Texture("Textures/sky.tga");
	skyTexture.LoadTexture();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.3f, 0.3f,
								0.0f, 0.0f, -1.0f);
//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 1.0f,
								3.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);							
	pointLightCount++;

		pointLights[1] = PointLight(1.0f, 0.33f, 0.0f,
								0.0f, 1.0f,
								-3.0f, 0.0f, 0.0f,
								0.3f, 0.4f, 0.1f);	
	pointLightCount++;
	//Ejercicio 2: agregar otra luz puntual

	/*Ejercicio 3: mover las dos luces de forma independiente con teclado y tomar 3 capturas de pantalla:
	1.- Dado iluminado con luz 1
	2.- Dado iluminado con luz 2
	3.- Dado iluminado con luz 1 y 2
	*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 200.0f);
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pointLights[0].SetPosition(mainWindow.getmueveLight1x(),mainWindow.getmueveLight1y(),mainWindow.getmueveLight1z());
		pointLights[1].SetPosition(mainWindow.getmueveLight2x(),mainWindow.getmueveLight2y(),mainWindow.getmueveLight2z());
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		//instanciar su dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 2.0f));
		model = glm::scale(model, glm::vec3(200.0f,200.0f, 200.0f));
		//model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(0.0f,0.0f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		skyTexture.UseTexture();
		meshList[3]->RenderMesh();

		//instanciar su dado de 10 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dado10Texture.UseTexture();
		meshList[4]->RenderMesh();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}