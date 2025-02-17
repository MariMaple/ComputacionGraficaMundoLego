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
	muevex = 2.0f;
	mueveluz1x = 15.0f;
	mueveluz1z = 15.0f;
	mueveluz2x = -15.0f;
	mueveluz2z = -15.0f;
	enciendeapaga = 20.0f;
	BailaLuz=false;
	apaga = false;
	salta = 0.0f;
	escondido = 0.0f;
	sombreros = 0.0f;
	vuelaconejo = 0.0f;

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
	mainWindow = glfwCreateWindow(width, height, "Primer ventana", NULL, NULL);

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
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}





void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}





	if (key == GLFW_KEY_C)
	{
		theWindow->escondido = 0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->escondido = 2.3f;
	}
	if (key == GLFW_KEY_9)
	{
		theWindow->sombreros= 2.0f;
	}
	if (key == GLFW_KEY_8)
	{
		theWindow->sombreros = 0;
	}


	if (key == GLFW_KEY_B)
	{
		theWindow-> BailaLuz=1;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow-> BailaLuz=0;
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->apaga= true;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->apaga = false;
	}

	if (key == GLFW_KEY_Z)
	{
		theWindow->salta=1;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->salta = 0;
	}
	if (key == GLFW_KEY_5)
	{
		theWindow->vuelaconejo = 1;
	}




	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex -= 0.5;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex += 0.5;
	}
	if (key == GLFW_KEY_F)
	{
		theWindow->mueveluz1x += 0.5;
	}

	if (key == GLFW_KEY_T)
	{
		theWindow->mueveluz1x -= 0.5;
	}

	if (key == GLFW_KEY_J)
	{
		theWindow->mueveluz2x -= 0.5;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->mueveluz2x += 0.5;
	}

	if (key == GLFW_KEY_I)
	{
		theWindow->mueveluz2z += 0.5;
	}
	if (key == GLFW_KEY_K)
	{
		theWindow->mueveluz2z -= 0.5;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->enciendeapaga = 20.0f;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->enciendeapaga = 0.0f;
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

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
