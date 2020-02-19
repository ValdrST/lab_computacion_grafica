#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO[3], VBO[3], shader; //Se crea un VAO y un VBO por cada objeto (letra) a dibujar.


//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
layout (location =1) in vec3 color;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
uniform vec4 Vcolor = vec4(1.0); // se a�ade variable uniform \n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = Vcolor;	 								\n\
}";



void CrearTriangulo(){
	GLfloat letraV[] = {
		// V
		-0.9f, 0.5f,0.0f,
		-0.8f,-0.3f, 0.0f,
		-0.8f,-0.4f,0.0f,

		-0.8f, -0.4f,0.0f,
		-0.9f,0.5f, 0.0f,
		-0.9f,0.3f,0.0f,

		-0.8f, -0.4f,0.0f,
		-0.8f,-0.3f, 0.0f,
		-0.7f,0.5f,0.0f,

		-0.8f, -0.4f,0.0f,
		-0.7f,0.3f, 0.0f,
		-0.7f,0.5f,0.0f
	};
	GLfloat letraR[] = {
		//R
		-0.6f,0.5f,0.0f,
		-0.3f,0.5f,0.0f,
		-0.3f,0.3f,0.0f,

		-0.3f,0.3f,0.0f,
		-0.6f,0.3f,0.0f,
		-0.6f,0.5f,0.0f,

		-0.4f,0.3f,0.0f,
		-0.3f,0.3f,0.0f,
		-0.5f,0.0f,0.0f,

		-0.5f,0.0f,0.0f,
		-0.5f,0.15f,0.0f,
		-0.4f,0.3f,0.0f,

		-0.6f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		-0.6f,-0.5f,0.0f,

		-0.5f,-0.5f,0.0f,
		-0.6f,-0.5f,0.0f,
		-0.5f,0.5f,0.0f,

		-0.4f, -0.5f,0.0f,
		-0.3f,-0.5f,0.0f,
		-0.5f,-0.0f,0.0f,

		-0.5f,-0.15f,0.0f,
		-0.5f,-0.0f,0.0f,
		-0.3f,-0.5f,0.0f
	};
	GLfloat letraA[] = {
		//A
		0.0f, -0.7f, -0.0f,
		0.1f, -0.7f, 0.0f,
		0.2f, 0.3f, 0.0f,

		0.0f, -0.7f, -0.0f,
		0.2f, 0.3f, 0.0f,
		0.2f, 0.4f, 0.0f,


		0.3f, -0.7f, -0.0f,
		0.25f, -0.7f, 0.0f,
		0.2f, 0.3f, 0.0f,

		0.3f, -0.7f, -0.0f,
		0.25f, -0.7f, 0.0f,
		0.2f, 0.3f, 0.0f,

		.3f, -0.7f, -0.0f,
		0.2f, 0.4f, 0.0f,
		0.2f, 0.4f, 0.0f,

		0.1f, -0.1f, -0.0f,
		0.25f, -0.1f, 0.0f,
		0.25f, -0.2f, 0.0f,

		0.1f, -0.2f, -0.0f,
		0.1f, -0.1f, 0.0f,
		0.25f, -0.2f, 0.0f

	};

	glGenVertexArrays(3, &VAO[0]); //generar 1 VAO
	glBindVertexArray(VAO[0]);//asignar VAO

	glGenBuffers(3, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(letraV), letraV, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es est�tico pues no se modificar�n los valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a v�rtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(3, &VAO[1]); //generar 1 VAO
	glBindVertexArray(VAO[1]);//asignar VAO

	glGenBuffers(3, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(letraR), letraR, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es est�tico pues no se modificar�n los valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a v�rtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(3, &VAO[2]); //generar 1 VAO
	glBindVertexArray(VAO[2]);//asignar VAO

	glGenBuffers(3, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(letraA), letraA, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es est�tico pues no se modificar�n los valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a v�rtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
//Funci�n para agregar los shaders a la tarjeta gr�fica
//the Program recibe los datos de theShader
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el c�digo
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevenci�n de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el c�digo a la tarjeta gr�fica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader){
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gr�fica
	 //verificaciones y prevenci�n de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main(){
	//Inicializaci�n de GLFW
	if (!glfwInit()){
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow){
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK){
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear tr�angulo
	CrearTriangulo();
	CompileShaders();
	GLint locColor = glGetUniformLocation(shader, "Vcolor"); // Se define la variable uniform que se trabajara dentro del shader

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow)){
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 57);
		glBindVertexArray(0);
		glUniform4f(locColor, 1.0f, 0.0f, 0.0f, 1.0f); // Se a�ade los colores a la variable uniform
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 57);
		glBindVertexArray(0); 
		glUniform4f(locColor, 0.0f, 1.0f, 0.0f, 1.0f);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 57);
		glBindVertexArray(0);
		glUniform4f(locColor, 0.0f, 0.0f, 1.0f, 1.0f);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}
	return 0;
}