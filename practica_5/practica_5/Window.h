#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getcodo() { return codo; }
	GLfloat gethombro() { return hombro; }
	GLfloat getmuneca() { return muneca; }
	GLfloat getfalange_indice() { return falange_indice; }
	GLfloat getfalange_indice_z() { return falange_indice_z; }
	GLfloat getfalangina_indice() { return falangina_indice; }
	GLfloat getfalangeta_indice() { return falangeta_indice; }
	GLfloat getfalange_medio() { return falange_medio; }
	GLfloat getfalange_medio_z() { return falange_medio_z; }
	GLfloat getfalangina_medio() { return falangina_medio; }
	GLfloat getfalangeta_medio() { return falangeta_medio; }
	GLfloat getfalange_anular() { return falange_anular; }
	GLfloat getfalangina_anular() { return falangina_anular; }
	GLfloat getfalangeta_anular() { return falangeta_anular; }
	GLfloat getfalange_menique() { return falange_menique; }
	GLfloat getfalange_menique_z() { return falange_menique_z; }
	GLfloat getfalangina_menique() { return falangina_menique; }
	GLfloat getfalangeta_menique() { return falangeta_menique; }
	GLfloat getfalange_pulgar() { return falange_pulgar; }
	GLfloat getfalange_pulgar_z() { return falange_pulgar_z; }
	GLfloat getfalange_pulgar_x() { return falange_pulgar_x; }
	GLfloat getfalangeta_pulgar() { return falangeta_pulgar; }
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat codo, hombro, muneca, falange_indice, falange_indice_z, falangina_indice, 
	falangeta_indice, rotay, falange_medio, falange_medio_z, falangina_medio, falangeta_medio,
	falange_anular, falangina_anular, falangeta_anular, falange_menique, falangina_menique, falangeta_menique, 
	falange_menique_z , falange_pulgar, falange_pulgar_z, falange_pulgar_x, falangeta_pulgar;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
};

