#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotay = 0.0f;
codo = 0.0f;
	hombro = 0.0f;
	muneca = 0.0f;
	falange_indice = 0.0f;
	falangina_indice = 0.0f;
	falangeta_indice = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
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
	mainWindow = glfwCreateWindow(width, height, "Practica 5 Modelado Jerarquico", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_E)
	{
		theWindow->hombro += 10.0;
	}
	if (key== GLFW_KEY_R)
	{
		theWindow->codo += 10.0;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->muneca += 10.0;
	}
	if (key == GLFW_KEY_7)
	{
		theWindow->falange_indice += 10.0;
	}
	if (key == GLFW_KEY_8)
	{
		theWindow->falangina_indice += 10.0;
	}
	if (key == GLFW_KEY_9)
	{
		theWindow->falangeta_indice += 10.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->falange_medio += 10.0;
	}
	if (key == GLFW_KEY_I)
	{
		theWindow->falangina_medio += 10.0;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->falangeta_medio += 10.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->falange_anular += 10.0;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->falangina_anular += 10.0;
	}
	if (key == GLFW_KEY_K)
	{
		theWindow->falangeta_anular += 10.0;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->falange_menique += 10.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->falangina_menique += 10.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->falangeta_menique += 10.0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->falange_pulgar += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->falangeta_pulgar += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	// Reset mano
	if(key == GLFW_KEY_F8){
		theWindow->codo = 0.0;
		theWindow->hombro = 0.0;
		theWindow->muneca = 0.0;
		theWindow->falange_indice = 0.0;
		theWindow->falange_indice_z = 0.0;
		theWindow->falangina_indice = 0.0;
		theWindow->falangeta_indice = 0.0;
		theWindow->rotay = 0.0;
		theWindow->falange_medio = 0.0; 
		theWindow->falange_medio_z = 0.0;
		theWindow->falangina_medio = 0.0;
		theWindow->falangeta_medio = 0.0;
		theWindow->falange_anular = 0.0;
		theWindow->falangina_anular = 0.0;
		theWindow->falangeta_anular = 0.0;
		theWindow->falange_menique = 0.0;
		theWindow->falangina_menique = 0.0;
		theWindow->falangeta_menique = 0.0;
		theWindow->falange_menique_z = 0.0;
		theWindow->falange_pulgar= 0.0;
		theWindow->falange_pulgar_x = 0.0;
		theWindow->falange_pulgar_z = 0.0;
		theWindow->falangeta_pulgar = 0.0;
	}
	// expresion rock
	if(key == GLFW_KEY_F9){
		theWindow->codo = 30.0;
		theWindow->hombro = 0.0;
		theWindow->muneca = 30.0;
		theWindow->falange_pulgar_z = -30.0;
		theWindow->falange_pulgar= 0.0;
		theWindow->falangeta_pulgar = 0.0;
		theWindow->falange_indice = 0.0;
		theWindow->falangina_indice = 0.0;
		theWindow->falangeta_indice = 0.0;
		theWindow->falange_indice_z = 10.0;
		theWindow->falange_menique_z = -10.0;
		theWindow->falange_menique = 0.0;
		theWindow->falangina_menique = 0.0;
		theWindow->falangeta_menique = 0.0;
		theWindow->falange_medio = 90.0;
		theWindow->falange_medio_z = 0.0;
		theWindow->falangina_medio = 90.0;
		theWindow->falangeta_medio = 90.0;
		theWindow->falange_anular = 90.0;
		theWindow->falange_pulgar_x = 0.0;
		theWindow->falangina_anular = 90.0;
		theWindow->falangeta_anular = 90.0;

	}
	// Expresion paz
	if(key == GLFW_KEY_F10){
		theWindow->hombro = 0.0;
		theWindow->codo = 30.0;
		theWindow->muneca = 30.0;
		theWindow->falange_pulgar_z = 0.0;
		theWindow->falange_pulgar= 90.0;
		theWindow->falangeta_pulgar = 90.0;
		theWindow->falange_indice = 0.0;
		theWindow->falangina_indice = 0.0;
		theWindow->falangeta_indice = 0.0;
		theWindow->falange_indice_z = 20.0;
		theWindow->falange_menique_z = 00.0;
		theWindow->falange_menique = 90.0;
		theWindow->falangina_menique = 90.0;
		theWindow->falangeta_menique = 90.0;
		theWindow->falange_medio = 0.0;
		theWindow->falange_medio_z = -10.0;
		theWindow->falangina_medio = 0.0;
		theWindow->falangeta_medio = 0.0;
		theWindow->falange_anular = 90.0;
		theWindow->falange_pulgar_x = 0.0;
		theWindow->falangina_anular = 90.0;
		theWindow->falangeta_anular = 90.0;

	}
	// Expresion like
	if(key == GLFW_KEY_F11){
		theWindow->hombro = -20.0;
		theWindow->codo = 20.0;
		theWindow->muneca = 10.0;
		theWindow->falange_indice = 90.0;
		theWindow->falange_indice_z = 0.0;
		theWindow->falangina_indice = 90.0;
		theWindow->falangeta_indice = 90.0;
		theWindow->rotay = 0.0;
		theWindow->falange_medio = 90.0; 
		theWindow->falange_medio_z = 0.0;
		theWindow->falangina_medio = 90.0;
		theWindow->falangeta_medio = 90.0;
		theWindow->falange_anular = 90.0;
		theWindow->falangina_anular = 90.0;
		theWindow->falangeta_anular = 90.0;
		theWindow->falange_menique = 90.0;
		theWindow->falangina_menique = 90.0;
		theWindow->falangeta_menique = 90.0;
		theWindow->falange_menique_z = 0.0;
		theWindow->falange_pulgar= 0.0;
		theWindow->falange_pulgar_z = 0.0;
		theWindow->falange_pulgar_x = 90.0;
		theWindow->falangeta_pulgar = 0.0;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
