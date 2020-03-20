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
	GLfloat getmueveLight1x(){ 
		GLfloat theChange = mueveLight1x; 
		mueveLight1x = 0.0f;
		return theChange;
	}
	GLfloat getmueveLight1y(){ 
		GLfloat theChange = mueveLight1y; 
		mueveLight1y = 0.0f;
		return theChange;
	}
	GLfloat getmueveLight1z(){ 
		GLfloat theChange = mueveLight1z; 
		mueveLight1z = 0.0f;
		return theChange;
	}
	GLfloat getmueveLight2x(){ 
		GLfloat theChange = mueveLight2x; 
		mueveLight2x = 0.0f;
		return theChange;
	}
	GLfloat getmueveLight2y(){ 
		GLfloat theChange = mueveLight2y; 
		mueveLight2y = 0.0f;
		return theChange;
	}
	GLfloat getmueveLight2z(){ 
		GLfloat theChange = mueveLight2z; 
		mueveLight2z = 0.0f;
		return theChange;
	}
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat mueveLight1x=0.0f;
	GLfloat mueveLight1y=0.0f;
	GLfloat mueveLight1z=0.0f;
	GLfloat mueveLight2x=0.0f;
	GLfloat mueveLight2y=0.0f;
	GLfloat mueveLight2z=0.0f;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

