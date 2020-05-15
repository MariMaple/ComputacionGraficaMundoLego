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
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmueveluz1x() { return mueveluz1x; }
	GLfloat getmueveluz1z() { return mueveluz1z; }
	GLfloat getmueveluz2x() { return mueveluz2x; }
	GLfloat getmueveluz2z() { return mueveluz2z; }
	GLfloat getenciendeapaga() { return enciendeapaga; }
	GLboolean getBailaLuz() { return BailaLuz; }
	GLboolean getapaga() { return apaga; }
	GLfloat getsalta() { return salta; }
	GLfloat	 getescondido() { return escondido; }
	GLfloat	 getsombrero() { return sombreros; }
	GLfloat getvuelaconejo() { return vuelaconejo; }

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
	GLfloat mueveluz1x;
	GLfloat mueveluz1z;
	GLfloat mueveluz2x;
	GLfloat mueveluz2z;
	GLfloat enciendeapaga;
	GLboolean BailaLuz;
	GLboolean apaga;
	GLfloat salta;
	GLfloat escondido;
	GLfloat sombreros;
	GLfloat vuelaconejo;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

