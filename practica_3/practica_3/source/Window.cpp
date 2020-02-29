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
	mainWindow = glfwCreateWindow(width, height, "Practica 3: Proyecciones y transformaciones geometricas", NULL, NULL);

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
	extern float valorX, valorY, valorZ;
	extern float persp;
	extern float rotX, rotY, rotZ;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_D)
	{
		rotY -= 1.0f;
	}
	if (key == GLFW_KEY_A)
	{
		rotY += 1.0f;
	}
	if (key == GLFW_KEY_S)
	{
		rotX -= 1.0f;
	}
	if (key == GLFW_KEY_W)
	{
		rotX += 1.0f;
	}
	if (key == GLFW_KEY_Z)
	{
		rotZ -= 1.0f;
	}
	if (key == GLFW_KEY_X)
	{
		rotZ += 1.0f;
	}
	if (key == GLFW_KEY_UP)
	{
		valorY += 0.10f;
	}
	if (key == GLFW_KEY_DOWN)
	{
		valorY -= .10f;
	}
	if (key == 93)
	{
		persp += 100.0f;
	}
	if (key == 47)
	{
		persp -= 100.0f;
	}
	if (key == GLFW_KEY_LEFT)
	{
		valorX -= .10f;
	}
	if (key == GLFW_KEY_RIGHT)
	{
		valorX += .10f;
	}
	if (key == GLFW_KEY_Q)
	{
		valorZ += 0.10f;
	}
	if (key == GLFW_KEY_E)
	{
		valorZ -= 0.10f;
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
			printf("posX: %f,\nposY:%f,\nposZ:%f\n rotX: %f,\nrotY:%f,\nrotZ:%f\n",valorX,valorY,valorZ,rotX,rotY,rotZ);
			printf("se solto la tecla %d'\n", key);
		}
	}
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
