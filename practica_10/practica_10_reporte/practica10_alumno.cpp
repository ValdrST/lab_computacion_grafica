/*
Semestre 2020-2
Pr�ctica 10: Animaci�n Sencilla
Pr�ctica 11: animaci�n compleja
Texturizado: se agrega c�digo para transparencia y blending en el shader
Skybox: Se agrega Skybox como textura ligada a la c�mara. 
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
//para probar el importer
//#include<assimp/Importer.hpp>

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

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
bool avanza;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;

Skybox skybox;

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
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
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

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

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
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};
	
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
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
	mainWindow = Window(1920, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture= Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt2.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/circuito_a.obj");
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.3f, 0.3f,
								0.0f, 0.0f, -1.0f);
//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
								0.0f, 1.0f,
								2.0f, 1.5f,1.5f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de faro trasera
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de faro frontal
	spotLights[4] = SpotLight(1.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	spotLights[5] = SpotLight(1.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	 //luz de helic�ptero
	

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");*/


	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	
	movCoche = 0.0f;
	float movCocheZ = 0.0f;
	float rotCoche = 0.0f;
	float rotCocheOffset = 5.0f;
	float rotCocheAlt = 0.0f;
	float rotCocheAllOffset = 5.0f;
	float altCoche = 0.0f;
	float giro_rueda = 0.0f;
	float dir_rueda = 0.0f;
	float dir_rueda_offset = 15.0f;
	float giro_offset = 90.0f;
	movOffset = 0.5f;
	float altOffset = 0.25f;
	int sentido_carro = 1;
	int checkpoint_1 = 0;
	glm::vec3 kitt_pos;
	glm::vec3 faro_pos;
	glm::vec3 faro_dir;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = 10*(now - lastTime); 
		lastTime = now;
		if(movCoche < 14.0f && sentido_carro == 1 && checkpoint_1 == 0){
			printf("estado 04\n");
			movCoche += movOffset * deltaTime;
		}else if(movCoche > 14.0f && sentido_carro == 1 && movCoche < 20.0f && checkpoint_1 == 0){
			printf("estado 05\n");
			movCoche += movOffset * deltaTime;
			altCoche += altOffset * deltaTime;
			if(rotCocheAlt > -23.0f){
				rotCocheAlt -= rotCocheAllOffset * deltaTime;
			}
		}else if(movCoche > 20.0f && movCoche < 29.0f && checkpoint_1 == 0){
			printf("estado 06\n");
			if(altCoche > 2.5f)
			altCoche -= altOffset * deltaTime;
			movCoche += movOffset * deltaTime;
			if(rotCocheAlt < 0.0f){
				rotCocheAlt += rotCocheAllOffset * deltaTime;
			}
		}else if(movCoche > 29.0f && movCoche < 35.0f && checkpoint_1 == 0){
			printf("estado 07\n");
			movCoche += movOffset * deltaTime;
			altCoche -= altOffset * deltaTime;
			if(rotCocheAlt < 23.0f){
				rotCocheAlt += rotCocheAllOffset * deltaTime;
			}
		}else if(movCoche > 35.0f && movCoche < 43.0f && checkpoint_1 == 0){
			printf("estado 08\n");
			movCoche += movOffset * deltaTime;
			if(altCoche < 0.0f){
				altCoche += altOffset * deltaTime;
			}
			if(rotCocheAlt > 0.0f){
				rotCocheAlt -= rotCocheAllOffset * deltaTime;
			}
		}
		else if(movCoche > 43.0f && movCoche < 50.0f && checkpoint_1 == 0){
			printf("estado 09\n");
			movCoche += movOffset * deltaTime;
			movCocheZ -= movOffset * deltaTime;
			if(rotCoche < 45.0f){
				 rotCoche += rotCocheOffset * deltaTime;
				if(dir_rueda < 45.0f)
					dir_rueda += dir_rueda_offset * deltaTime;
				else if(dir_rueda > 0.0f)
					dir_rueda -= dir_rueda_offset * deltaTime;
			}
		}else if(movCocheZ < -6.5f && movCocheZ > -15.0f && checkpoint_1 == 0){
			printf("estado 10\n");
			movCocheZ -= movOffset * deltaTime;
			if(rotCoche < 90.0f){
				rotCoche += rotCocheOffset * deltaTime;
			}
			if(dir_rueda > 0.0f)
				dir_rueda -= dir_rueda_offset * deltaTime;
		}else if(movCocheZ < -15.0f && movCocheZ > -23.0f && checkpoint_1 != 2){
			printf("estado 11\n");
			checkpoint_1 = 1;
			movCoche -= movOffset * deltaTime;
			movCocheZ -= movOffset * deltaTime;
			if(rotCoche < 145.0f){
				rotCoche += rotCocheOffset * deltaTime;
				if(dir_rueda < 45.0f)
					dir_rueda += dir_rueda_offset * deltaTime;
				else if(dir_rueda > 0.0f)
					dir_rueda -= dir_rueda_offset * deltaTime;
			}
		}else if(movCoche > 0.0f && movCocheZ < -23.0f  && checkpoint_1 == 1){
			printf("estado 12\n");
			movCoche -= movOffset * deltaTime;
			if(rotCoche < 180.0f){
				rotCoche += rotCocheOffset * deltaTime;
				if(dir_rueda > 0.0f)
					dir_rueda -= dir_rueda_offset * deltaTime;
			}
		}else if(movCoche > -43.0f && checkpoint_1 == 1){
			printf("estado 13\n");
			movCoche -= movOffset * deltaTime;
			if(dir_rueda > 0.0f)
				dir_rueda -= dir_rueda_offset * deltaTime;
		}else if(movCoche < -43.0f && movCoche > -46.0f && checkpoint_1 != 3){
			printf("estado 13n\n");
			checkpoint_1 = 2;
			movCoche -= movOffset * deltaTime;
			movCocheZ += movOffset * deltaTime;
			if(rotCoche < 270.0f){
				 rotCoche += rotCocheOffset * deltaTime;
				if(dir_rueda < 45.0f)
					dir_rueda += dir_rueda_offset * deltaTime;
				else if(dir_rueda > 0.0f)
					dir_rueda -= dir_rueda_offset * deltaTime;
			}
		}else if(movCocheZ > -23.5f && movCocheZ < -7.5f){
			printf("estado 14\n");
			movCocheZ += movOffset * deltaTime;
			if(rotCoche < 270.0f){
				rotCoche += rotCocheOffset * deltaTime;
			}
			if(dir_rueda > 0.0f)
				dir_rueda -= dir_rueda_offset * deltaTime;
		}
		else if(movCocheZ > -7.5f && movCocheZ < 0.0f && checkpoint_1 != 0){
			checkpoint_1 = 3;
			printf("estado 15\n");
			movCocheZ += movOffset * deltaTime;
			movCoche += movOffset * deltaTime;
			if(rotCoche < 360.0f){
				rotCoche += rotCocheOffset * deltaTime;
			}
			if(dir_rueda < 45.0f)
				dir_rueda += dir_rueda_offset * deltaTime;
			else if(dir_rueda > 0.0f)
				dir_rueda -= dir_rueda_offset * deltaTime;
		}else if(movCoche < 0.0f){
			
			printf("estado 16\n");
			movCoche += movOffset * deltaTime;
			if(rotCoche < 360.0f && rotCoche > 270.0f){
				rotCoche += rotCocheOffset * deltaTime;
			}else
				rotCoche = 0.0f;
			if(dir_rueda > 0.0f)
				dir_rueda -= dir_rueda_offset * deltaTime;
		}else if(checkpoint_1 == 3){
			checkpoint_1 = 0;
		}
		giro_rueda += giro_offset * deltaTime;
		printf("mov carro: %f\n",movCoche);
		printf("mov carro z: %f\n",movCocheZ);
		printf("alt carro: %f\n",altCoche);
		printf("rotCoccheAlt carro: %f\n",rotCocheAlt );
		printf("rot coche carro: %f\n",rotCoche);

		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		printf("lower_light: %f,%f,%f\nDireccion: %f %f %f\n Grados: %f %f %f\n",lowerLight.x,lowerLight.y,lowerLight.z,
		camera.getCameraDirection().x,camera.getCameraDirection().y,camera.getCameraDirection().z,
		glm::degrees(glm::acos(camera.getCameraDirection().x)),glm::degrees(glm::acos(camera.getCameraDirection().y)),glm::degrees(glm::acos(camera.getCameraDirection().z)));
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f,30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		
		model = glm::mat4(1.0);
		kitt_pos = glm::vec3(movCoche, -1.2f + altCoche, 0.5f+movCocheZ);
		faro_pos = glm::vec3(movCoche-1.0f, -1.2f + altCoche, 0.5f+movCocheZ);
		faro_dir = glm::vec3(glm::cos(rotCoche),glm::cos(rotCocheAlt),glm::sin(rotCoche));
		printf("%f,%f,%f\n %f %f %f\n",faro_dir.x,faro_dir.y,faro_dir.z,
				glm::degrees(glm::acos(faro_dir.x)),glm::acos(faro_dir.y),glm::acos(faro_dir.z));
		spotLights[2].SetPos(glm::vec3(movCoche-1.0f, -1.2f + altCoche, 0.5f+movCocheZ));
		spotLights[3].SetPos(glm::vec3(movCoche-1.0f,-1.2f + altCoche, -1.5f+movCocheZ));
		spotLights[4].SetPos(glm::vec3(movCoche-1.0f, -1.2f + altCoche, 0.5f+movCocheZ));
		spotLights[4].SetFlash(glm::vec3(movCoche-1.0f, -1.2f + altCoche, 0.5f+movCocheZ),faro_dir);
		spotLights[5].SetPos(glm::vec3(movCoche-1.0f, -1.2f + altCoche, -1.5f+movCocheZ));
		spotLights[5].SetFlash(glm::vec3(movCoche-1.0f, -1.2f + altCoche, 0.5f+movCocheZ),faro_dir);
		if(sentido_carro == 1){
			spotLights[4].SetColor(glm::vec3(0.0f,0.0f,0.0f));
			spotLights[5].SetColor(glm::vec3(0.0f,0.0f,0.0f));
			spotLights[2].SetColor(glm::vec3(0.0f,0.0f,0.0f));
			spotLights[3].SetColor(glm::vec3(0.0f,0.0f,0.0f));
		}
		else{
			spotLights[4].SetColor(glm::vec3(0.0f,0.0f,0.0f));
			spotLights[5].SetColor(glm::vec3(0.0f,0.0f,0.0f));
			spotLights[2].SetColor(glm::vec3(1.0f,1.0f,1.0f));
			spotLights[3].SetColor(glm::vec3(1.0f,1.0f,1.0f));
		}
		
		model = glm::translate(model, kitt_pos);
		model = glm::scale(model, glm::vec3(0.1f,0.1f, 0.1f));
		//model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, glm::radians(90.0f+rotCoche), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotCocheAlt), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kitt_M.RenderModel();
		model = modelaux;
		model = glm::translate(model,glm::vec3(-2.50f,0.0f,-10.0f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.4f,0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(90.0f+dir_rueda), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giro_rueda), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		model = modelaux;
		model = glm::translate(model,glm::vec3(-2.50f,0.0f,-27.0f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.4f,0.4f, 0.4f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giro_rueda), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		model = modelaux;
		model = glm::translate(model,glm::vec3(12.5f,0.0f,-10.0f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.4f,0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(90.0f+dir_rueda), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-giro_rueda), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		model = modelaux;
		model = glm::translate(model,glm::vec3(12.5f,0.0f,-27.0f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.4f,0.4f, 0.4f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-giro_rueda), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(2.0f, 2.0f, 0.0f);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.50f, -0.1f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino_M.RenderModel();

		//Agave �qu� sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
			glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}