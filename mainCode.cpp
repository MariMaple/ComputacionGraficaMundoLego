/*
Arguelles Macosay Mariana
Semestre 2020-2
Texturizado: se agrega código para transparencia y blending en el shader
Skybox: Se agrega Skybox como textura ligada a la cámara.
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <random>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"
#include "mainCode.h"

const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
float movHelicopter;
float rotaHelicoptero;
float helicoptero_mueve_z;
float helicoptero_mueve_y;
float helicoptero_mueve_x;
float gira_eje;
int cont_temp;
bool avanza;
bool tope;
bool bandera;
int var;
bool enciende;
float gira_conejo2 = 0.0;
bool apagado;
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

//Texturas
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Modelos
Model Helicoptero;
Model Helices;
Model Pino1;
Model Pino2;
Model Pino3;
Model Arbol;
Model Lampara1;
Model Lampara2;
Model Lampara3;
Model Bote;
Model Banca;
Model Kiosco;
Model Reja;
Model Piso;
Model Elfo;
Model Elfo_cabeza;
Model Elfo_brazo_derecho;
Model Elfo_brazo_izquierdo;
Model Elfo_pierna_izquierda;
Model Elfo_pierna_derecha;
Model Elfo_pecho;
Model Camino;
Model Kiosco_madera;
Model Kiosco_base;
Model Kiosco2;
Model Kiosco4;
Model Kiosco6;
Model Kiosco8;
Model Kiosco10;
Model Kiosco12;
Model Kiosco14;
Model Kiosco16;
Model Kiosco18;
Model Kiosco20;
Model Kiosco_columnas;
Model Kiosco_punta;
Model Escalones;
Model FocoRojo;
Model FocoAmarillo;
Model FocoAzul;
Model FocoVerde;
Model Sol;
Model Conejo;
Model sombrero;
Model raptor;
Model Arbusto;
Model Fuente;
Model Baño;
Model rex;
Model rex1;
Model volcano;
Model estego;
Model p;
Model m;
Model h;
Model terodactilo;

Skybox skybox;
Skybox skybox_dia;
Skybox skybox_noche;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;




//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

bool animacion = false;

//NEW// Keyframes
float posXconejo = -15.0f, posYconejo = -2.0f, posZconejo = -15.0f;
float	movConejo_x = 0.0f, movConejo_y = 0.0f, movConejo_z = 0.0f;
float giroConejosy = 0;

#define MAX_FRAMES 20
int i_max_steps = 28;
int i_curr_steps = 20;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movConejo_x;		//Variable para PosicionX
	float movConejo_y;		//Variable para PosicionY
	float movConejo_z;
	float movConejo_xInc;		//Variable para IncrementoX
	float movConejo_yInc;		//Variable para IncrementoY
	float movConejo_zInc;
	float giroConejosy;

	float giroConejoIncy;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 20;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movConejo_x = movConejo_x;
	KeyFrame[FrameIndex].movConejo_y = movConejo_y;
	KeyFrame[FrameIndex].movConejo_z = movConejo_z;
	KeyFrame[FrameIndex].giroConejosy = giroConejosy;

	FrameIndex++;
}

void resetElements(void)
{

	movConejo_x = KeyFrame[0].movConejo_x;
	movConejo_y = KeyFrame[0].movConejo_y;
	movConejo_z = KeyFrame[0].movConejo_z;
	giroConejosy = KeyFrame[0].giroConejosy;

	
}

void interpolation(void)
{
	KeyFrame[playIndex].movConejo_xInc = (KeyFrame[playIndex + 1].movConejo_x - KeyFrame[playIndex].movConejo_x) / i_max_steps;
	KeyFrame[playIndex].movConejo_yInc = (KeyFrame[playIndex + 1].movConejo_y - KeyFrame[playIndex].movConejo_y) / i_max_steps;
	KeyFrame[playIndex].movConejo_zInc = (KeyFrame[playIndex + 1].movConejo_z - KeyFrame[playIndex].movConejo_z) / i_max_steps;
	KeyFrame[playIndex].giroConejoIncy = (KeyFrame[playIndex + 1].giroConejosy - KeyFrame[playIndex].giroConejosy) / i_max_steps;
	
	
}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movConejo_x += KeyFrame[playIndex].movConejo_xInc;
			movConejo_y += KeyFrame[playIndex].movConejo_yInc;
			movConejo_z += KeyFrame[playIndex].movConejo_zInc;
			giroConejosy += KeyFrame[playIndex].giroConejoIncy;
			i_curr_steps++; 
		}

	}
}

/* FIN KEYFRAMES*/





int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//Importacion de Texturas
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Importacion de Modelos
	Helicoptero = Model();
	Helicoptero.LoadModel("Models/Helicoptero.obj");
	Helices = Model();
	Helices.LoadModel("Models/Helices.obj");
	Banca = Model();
	Banca.LoadModel("Models/banca.obj");
	Piso = Model();
	Piso.LoadModel("Models/piso.obj");
	Arbol = Model();
	Arbol.LoadModel("Models/arbol_lego.obj");
	Bote = Model();
	Bote.LoadModel("Models/botedebasura.obj");
	Camino = Model();
	Camino.LoadModel("Models/camino.obj");
	Lampara1 = Model();
	Lampara1.LoadModel("Models/lampara.obj");
	Lampara2 = Model();
	Lampara2.LoadModel("Models/lampara2.obj");
	Lampara3 = Model();
	Lampara3.LoadModel("Models/lampara3.obj");
	Pino3 = Model();
	Pino3.LoadModel("Models/pino_lego_realista.obj");
	Pino2 = Model();
	Pino2.LoadModel("Models/pinolegoplano.obj");
	Pino1 = Model();
	Pino1.LoadModel("Models/pinolego.obj");

	//Modelos del kiosco
	Kiosco_madera = Model();
	Kiosco_madera.LoadModel("Models/kioscoCuadroCafeobj.obj");
	Kiosco_base = Model();
	Kiosco_base.LoadModel("Models/basedekiosco.obj");
	Kiosco2 = Model();
	Kiosco2.LoadModel("Models/Techo2x2.obj");
	Kiosco4 = Model();
	Kiosco4.LoadModel("Models/Techo2x4.obj");
	Kiosco6 = Model();
	Kiosco6.LoadModel("Models/Techo2x6.obj");
	Kiosco8 = Model();
	Kiosco8.LoadModel("Models/Techo2x8.obj");
	Kiosco10 = Model();
	Kiosco10.LoadModel("Models/Techo2x10.obj");
	Kiosco12 = Model();
	Kiosco12.LoadModel("Models/Techo2x12.obj");
	Kiosco14 = Model();
	Kiosco14.LoadModel("Models/Techo2x14.obj");
	Kiosco16 = Model();
	Kiosco16.LoadModel("Models/Techo2x16.obj");
	Kiosco18 = Model();
	Kiosco18.LoadModel("Models/Techo2x18.obj");
	Kiosco20 = Model();
	Kiosco20.LoadModel("Models/Techo2x20.obj");
	Kiosco_columnas = Model();
	Kiosco_columnas.LoadModel("Models/columnas.obj");
	Kiosco_punta = Model();
	Kiosco_punta.LoadModel("Models/Cubo_lego_4x4.obj");
	Escalones = Model();
	Escalones.LoadModel("Models/Escalones.obj");
	FocoAmarillo = Model();
	FocoAmarillo.LoadModel("Models/FocoAmarillo.obj");
	FocoAzul = Model();
	FocoAzul.LoadModel("Models/FocoAzul.obj");
	FocoVerde = Model();
	FocoVerde.LoadModel("Models/FocoVerde.obj");
	FocoRojo = Model();
	FocoRojo.LoadModel("Models/FocoRojo.obj");
	Sol = Model();
	Sol.LoadModel("Models/sol.obj");
	Conejo = Model();
	Conejo.LoadModel("Models/Conejo.obj");
	raptor = Model();
	raptor.LoadModel("Models/raptor.obj");
	Arbusto = Model();
	Arbusto.LoadModel("Models/Arbusto.obj");
	Baño = Model();
	Baño.LoadModel("Models/Bano.obj");
	Fuente = Model();
	Fuente.LoadModel("Models/Fuente.obj");
	Reja = Model();
	Reja.LoadModel("Models/Cerca.obj");
	rex = Model();
	rex.LoadModel("Models/rex.obj");
	rex1 = Model();
	rex1.LoadModel("Models/rex1.obj");
	volcano = Model();
	volcano.LoadModel("Models/volcano.obj");
	estego = Model();
	estego.LoadModel("Models/estego.obj");
	p = Model();
	p.LoadModel("Models/p.obj");
	m = Model();
	m.LoadModel("Models/m.obj");

	h = Model();
	h.LoadModel("Models/h.obj");

	//Elfo
	Elfo_cabeza = Model();
	Elfo_cabeza.LoadModel("Models/Elfo_cabeza.obj");
	Elfo_brazo_derecho = Model();
	Elfo_brazo_derecho.LoadModel("Models/Elfo_brazo_derecho.obj");
	Elfo_brazo_izquierdo = Model();
	Elfo_brazo_izquierdo.LoadModel("Models/Elfo_brazo_izquierdo.obj");
	Elfo_pierna_derecha = Model();
	Elfo_pierna_derecha.LoadModel("Models/Elfo_pierna_derecha.obj");
	Elfo_pierna_izquierda = Model();
	Elfo_pierna_izquierda.LoadModel("Models/Elfo_pierna_izquierda.obj");
	Elfo_pecho = Model();
	Elfo_pecho.LoadModel("Models/Elfo_pecho.obj");
	sombrero = Model();
	sombrero.LoadModel("Models/Sombrero.obj");
	terodactilo = Model();
	terodactilo.LoadModel("Models/terodactilo.obj");




	//Elfo completo 
	Elfo = Model();
	Elfo.LoadModel("Models/Elfo_2.obj");

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		2.0f, 1.0f,
		-110.0f, 2.0f, -44.35f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		2.0f, 1.0f,
		-102.0f, 1.6f, -43.80f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	pointLights[3] = PointLight(0.0f, 0.0f, 1.0f,
		2.0f, 1.0f,
		-104.0f, 1.7f, -43.8f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	pointLights[4] = PointLight(1.0f, 1.0f, 1.0f,
		2.0f, 1.0f,
		-120.0f, 1.7f, -43.8f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	pointLights[5] = PointLight(1.0f, 1.0f, 1.0f,
		2.0f, 1.0f,
		-101.0f, 1.6f, -37.8f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	//luz azul
	pointLights[6] = PointLight(0.0f, 0.0f, 1.0f,
		2.0f, 1.0f,
		-110.0f, 1.8f, -37.8f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	pointLights[7] = PointLight(1.0f, 1.0f, 0.0f,
		2.0f, 1.0f,
		-115.0f, 1.8f, -37.8f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	pointLights[8] = PointLight(1.0f, 1.0f, 1.0f,
		2.0f, 1.0f,
		-120.0f, 1.8f, -38.0f,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;

	// luz central kisoco
	pointLights[9] = PointLight(1.0f, 1.0f, 1.0f,
		2.0f, 1.0f,
		-90.75f, 4.00f, -40.248,
		0.9f, 0.9f, 0.2f);
	pointLightCount++;





	//luces spotlight
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
	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[4] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[5] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[6] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[7] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[8] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[9] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[10] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[11] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[12] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[13] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	spotLights[14] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	glm::vec3 ConejoMove = glm::vec3(2.0f, 0.0f, 0.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/ri.tga");
	skyboxFaces.push_back("Textures/Skybox/l.tga");
	skyboxFaces.push_back("Textures/Skybox/b.tga");
	skyboxFaces.push_back("Textures/Skybox/u.tga");
	skyboxFaces.push_back("Textures/Skybox/r.tga");
	skyboxFaces.push_back("Textures/Skybox/f.tga");

	skybox_dia = Skybox(skyboxFaces);


	skyboxFaces.clear();
	skyboxFaces.push_back("Textures/Skybox/ri_n.tga");
	skyboxFaces.push_back("Textures/Skybox/l_n.tga");
	skyboxFaces.push_back("Textures/Skybox/b_n.tga");
	skyboxFaces.push_back("Textures/Skybox/u_n.tga");
	skyboxFaces.push_back("Textures/Skybox/r_n.tga");
	skyboxFaces.push_back("Textures/Skybox/f_n.tga");

	skybox_noche = Skybox(skyboxFaces);



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = -15.0f;
	movOffset = 1.0f;
	movHelicopter = 0.0f;
	helicoptero_mueve_z = 0.0f;
	helicoptero_mueve_y = 0.0f;
	helicoptero_mueve_x = 0.0f;
	gira_eje = -90;
	cont_temp = 0;
	tope = 0;
	var = -480.0f;
	bandera = 0;
	int cambio=1;
	int cambio2 = 1;
	//randoms:
	float rand1 = rand() % 16;
	float rand2 = rand() % 16;
	float rand3 = rand() % 16;
	float rand4 = rand() % 16;
	float rand5 = rand() % 16;
	float rand6 = rand() % 16;
	float rand7 = rand() % 16;
	float rand8 = rand() % 16;
	float rand9 = rand() % 16;
	float rand10 = rand() % 16;
	float rand11 = rand() % 16;
	float rand12 = rand() % 16;
	float rand13 = rand() % 16;
	float rand14 = rand() % 16;

	float rand_pos_x1 = -95 + rand() % 9;
	float rand_pos_y1 = 1 + rand() % 5;
	float rand_pos_z1 = -44 + rand() % 9;

	float rand_pos_x2 = -95 + rand() % 9;
	float rand_pos_y2 = 1 + rand() % 5;
	float rand_pos_z2 = -44 + rand() % 9;
	float rand_pos_x3 = -95 + rand() % 9;
	float rand_pos_y3 = 1 + rand() % 5;
	float rand_pos_z3 = -44 + rand() % 9;
	float rand_pos_x4 = -95 + rand() % 9;
	float rand_pos_y4 = 1 + rand() % 5;
	float rand_pos_z4 = -44 + rand() % 9;
	float rand_pos_x5 = -95 + rand() % 9;
	float rand_pos_y5 = 1 + rand() % 5;
	float rand_pos_z5 = -44 + rand() % 9;
	float rand_pos_x6 = -95 + rand() % 9;
	float rand_pos_y6 = 1 + rand() % 5;
	float rand_pos_z6 = -44 + rand() % 9;
	float rand_pos_x7 = -95 + rand() % 9;
	float rand_pos_y7 = 1 + rand() % 5;
	float rand_pos_z7 = -44 + rand() % 9;
	float rand_pos_x8 = -95 + rand() % 9;
	float rand_pos_y8 = 1 + rand() % 5;
	float rand_pos_z8 = -44 + rand() % 9;
	float rand_pos_x9 = -95 + rand() % 9;
	float rand_pos_y9 = 1 + rand() % 5;
	float rand_pos_z9 = -44 + rand() % 9;
	float rand_pos_x10 = -95 + rand() % 9;
	float rand_pos_y10 = 1 + rand() % 5;
	float rand_pos_z10 = -44 + rand() % 9;
	float rand_pos_x11 = -95 + rand() % 9;
	float rand_pos_y11 = 1 + rand() % 5;
	float rand_pos_z11 = -44 + rand() % 9;
	float rand_pos_x12 = -95 + rand() % 9;
	float rand_pos_y12 = 1 + rand() % 5;
	float rand_pos_z12 = -44 + rand() % 9;
	float rand_pos_x13 = -95 + rand() % 9;
	float rand_pos_y13 = 1 + rand() % 5;
	float rand_pos_z13 = -44 + rand() % 9;
	float rand_pos_x14 = -95 + rand() % 9;
	float rand_pos_y14 = 1 + rand() % 5;
	float rand_pos_z14 = -44 + rand() % 9;

	float saltax = 0.0f;
	float saltay = 0.0f;
	float saltaz = 0.0f;
	float gira_conejo = 0;
	float conejoSalta = 0.0f;
	float contador_conejo=0.0f;
	float contador_conejo2 = 0.0f;
	float contador_conejo3 = 0.0f;
	bool conejo=0;
	float VuelaConejo=0.0f;
	int CuentaConejo = 0;

	apagado = true;

	int tempo=0;
	bool dia = false;
	int girahelices = 0;
	skybox = skybox_dia;
	int cont_tiempo_dia_noche = 2499;
	int cont_espectculo_de_luces = 1;
	bool helicoptero_avanza = true;


	KeyFrame[0].movConejo_x = 0.0f;
	KeyFrame[0].movConejo_y = 2.0f;
	KeyFrame[0].movConejo_z = 0.0f;
	KeyFrame[0].giroConejosy = 0;


	KeyFrame[1].movConejo_x = 0.0f;
	KeyFrame[1].movConejo_y = 0.0f;
	KeyFrame[1].movConejo_z = 0.0f;
	KeyFrame[1].giroConejosy = 0;


	KeyFrame[2].movConejo_x = 2.0f;
	KeyFrame[2].movConejo_y = 2.0f;
	KeyFrame[2].movConejo_z = 0.0f;
	KeyFrame[2].giroConejosy = 0.0f;


	KeyFrame[3].movConejo_x = 4.0f;
	KeyFrame[3].movConejo_y = 0.0f;
	KeyFrame[3].movConejo_z = 0.0f;
	KeyFrame[3].giroConejosy = 0;


	KeyFrame[4].movConejo_x = 6.0f;
	KeyFrame[4].movConejo_y = 2.0f;
	KeyFrame[4].movConejo_z = 2.0f;
	KeyFrame[4].giroConejosy = -45;


	KeyFrame[5].movConejo_x = 8.0f;
	KeyFrame[5].movConejo_y = 0.0f;
	KeyFrame[5].movConejo_z = 4.0f;
	KeyFrame[5].giroConejosy = -45;

	KeyFrame[6].movConejo_x = 9.0f;
	KeyFrame[6].movConejo_y = 1.0f;
	KeyFrame[6].movConejo_z = 5.0f;
	KeyFrame[6].giroConejosy = -90;

	KeyFrame[7].movConejo_x = 9.0f;
	KeyFrame[7].movConejo_y = 0.0f;
	KeyFrame[7].movConejo_z = 6.0f;
	KeyFrame[7].giroConejosy = -90;

	KeyFrame[8].movConejo_x = 9.0f;
	KeyFrame[8].movConejo_y = 2.0f;
	KeyFrame[8].movConejo_z = 8.0f;
	KeyFrame[8].giroConejosy = -90;

	KeyFrame[9].movConejo_x = 9.0f;
	KeyFrame[9].movConejo_y = 0.0f;
	KeyFrame[9].movConejo_z = 8.0f;
	KeyFrame[9].giroConejosy = -90;
	

	KeyFrame[10].movConejo_x = 9.0f;
	KeyFrame[10].movConejo_y = 2.0f;
	KeyFrame[10].movConejo_z = 10.0f;
	KeyFrame[10].giroConejosy = -90;



	KeyFrame[11].movConejo_x = 9.0f;
	KeyFrame[11].movConejo_y = 0.0f;
	KeyFrame[11].movConejo_z = 10.0f;
	KeyFrame[11].giroConejosy = -90;


	//lega a su madriguera
	KeyFrame[12].movConejo_x = 9.0f;
	KeyFrame[12].movConejo_y = -2.0f;
	KeyFrame[12].movConejo_z = 10.0f;
	KeyFrame[12].giroConejosy = -90;





	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		// movimiento del coche

		if (cont_tiempo_dia_noche % 2500 == 0) {
			if (dia) {
				skybox = skybox_noche;
				mainLight.SetOnOffDir( 0.01f, 0.02f);
				pointLights[1].SetOnOff(2.0f, 1.0f);
				pointLights[2].SetOnOff(2.0f, 1.0f);
				pointLights[3].SetOnOff(2.0f, 1.0f);
				pointLights[4].SetOnOff(2.0f, 1.0f);
				pointLights[5].SetOnOff(2.0f, 1.0f);
				pointLights[6].SetOnOff(2.0f, 1.0f);
				pointLights[7].SetOnOff(2.0f, 1.0f);
				pointLights[8].SetOnOff(2.0f, 1.0f);
				pointLights[9].SetOnOff(2.0f, 1.0f);
				spotLights[1].SetEdge(15);
				spotLights[2].SetEdge(15);
				spotLights[3].SetEdge(15);
				spotLights[4].SetEdge(15);
				spotLights[5].SetEdge(15);
				spotLights[6].SetEdge(15);
				spotLights[7].SetEdge(15);
				spotLights[8].SetEdge(15);
				spotLights[9].SetEdge(15);
				spotLights[10].SetEdge(15);
				spotLights[11].SetEdge(15);
				spotLights[12].SetEdge(15);
				spotLights[13].SetEdge(15);
				spotLights[14].SetEdge(15);
			}
			else {
				skybox = skybox_dia;
				mainLight.SetOnOffDir(0.3f, 0.3f);
				pointLights[1].SetOnOff(0.0f, 0.0f);
				pointLights[2].SetOnOff(0.0f, 0.0f);
				pointLights[3].SetOnOff(0.0f, 0.0f);
				pointLights[4].SetOnOff(0.0f, 0.0f);
				pointLights[5].SetOnOff(0.0f, 0.0f);
				pointLights[6].SetOnOff(0.0f, 0.0f);
				pointLights[7].SetOnOff(0.0f, 0.0f);
				pointLights[8].SetOnOff(0.0f, 0.0f);
				pointLights[9].SetOnOff(0.0f, 0.0f);
				spotLights[1].SetEdge(0);
				spotLights[2].SetEdge(0);
				spotLights[3].SetEdge(0);
				spotLights[4].SetEdge(0);
				spotLights[5].SetEdge(0);
				spotLights[6].SetEdge(0);
				spotLights[7].SetEdge(0);
				spotLights[8].SetEdge(0);
				spotLights[9].SetEdge(0);
				spotLights[10].SetEdge(0);
				spotLights[11].SetEdge(0);
				spotLights[12].SetEdge(0);
				spotLights[13].SetEdge(0);
				spotLights[14].SetEdge(0);

			}
			dia = !dia; //para hace cambio si es 1 se volvera 0, si es 0 se volvera 1
		}

		cont_tiempo_dia_noche += 1;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		
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
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//luces
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 modelaux(1.0);
		glm::mat4 model(1.0);

		//piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.083f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Piso.RenderModel();

		//banca 1 y 2 bote de basura 1 y 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca.RenderModel();
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bote.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 50.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f,1.0f,0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 60.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bote.RenderModel();
	
		//Arbol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -2.0f, 200.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -2.0f, 150.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();

		
		modelaux = model;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -60.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -80.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();

		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 60.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();

		//Pino2
	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, 200.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, -2.0f, 170.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino2.RenderModel();

		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 60.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino2.RenderModel();

		//Camino
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-119.0f, -2.0f, -40.2f));
		model = glm::scale(model, glm::vec3(0.40f, 0.43f, 0.44f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 32.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino.RenderModel();

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-112.0f, -2.0f, -43.25f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara1.RenderModel();
		model = glm::translate(model, glm::vec3(24.5f, 0.0f, 40.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara1.RenderModel();
		model = glm::translate(model, glm::vec3(-24.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(24.5f, 0.0f, -40.7f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(24.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara3.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -40.7f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara3.RenderModel();


		//pino lego plano
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino2.RenderModel();
	
		

		//pino raro
		
	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -2.0f, -130.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		
		
		modelaux = model;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-250.0f, -2.0f, -130.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();


		//base parte de abajo
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel(); model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		



		// lateral derecho de los arboles
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -2.0f, 320.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();


		modelaux = model;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino1.RenderModel();

		

		//pino lego realista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pino3.RenderModel();

		//kiosco por modelado jerarquico
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.75f, 7.25f, -40.248));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_punta.RenderModel();
		modelaux = model;

		model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f, -4.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco2.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.5f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoAzul.RenderModel();

		//foco //acomodado
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoVerde.RenderModel();
		model = modelaux;



		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco4.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco4.RenderModel();
		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, .0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco4.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco4.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, -4.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco4.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, -4.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco4.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(4.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco6.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco6.RenderModel();
		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(4.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco6.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(4.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco6.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(5.0f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco8.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.0f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco8.RenderModel();
		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.0f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco8.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.5f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoAmarillo.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.0f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco8.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(6.0f, -5.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco10.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.0f, -5.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco10.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.5f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoAmarillo.RenderModel();
		model = modelaux;

		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(6.0f, -5.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco10.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(6.0f, -5.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco10.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(7.0f, -6.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco12.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.0f, -6.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco12.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(1.5f, -4.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoAzul.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoRojo.RenderModel();

		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(7.0f, -6.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco12.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(7.0f, -6.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco12.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.5f, -5.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoVerde.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoVerde.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(8.0f, -7.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco14.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoVerde.RenderModel();
		model = modelaux;
		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, -7.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco14.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoVerde.RenderModel();
		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(8.0f, -7.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco14.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(0.5f, -4.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoAmarillo.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(8.0f, -7.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco14.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(9.0f, -8.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco16.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, -8.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco16.RenderModel();

		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(9.0f, -8.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco16.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(9.0f, -8.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco16.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(2.5f, -4.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoAzul.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoRojo.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(10.0f, -9.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco18.RenderModel();
		//foco
		model = glm::translate(model, glm::vec3(1.5f, -4.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FocoRojo.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, -9.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco18.RenderModel();
		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(10.0f, -9.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco18.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(10.0f, -9.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco18.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(11.0f, -10.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco20.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.0f, -10.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco20.RenderModel();
		model = modelaux;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(11.0f, -10.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco20.RenderModel();
		model = modelaux;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(11.0f, -10.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco20.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(2.0f, -1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(-2.0f, -1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(-2.0f, -1.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = modelaux;

		model = glm::translate(model, glm::vec3(2.0f, -1.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_madera.RenderModel();
		model = modelaux;


		//columnas del kiosco
		model = glm::translate(model, glm::vec3(0.0f, -19.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_columnas.RenderModel();

		//base del kiosco esta ligado a las columnas por modelado jerarquico 
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_base.RenderModel();

		//Escaleras, estan unidas por modelo jerarquico a la base
		model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Escalones.RenderModel();
		model = modelaux;

		spotLights[10].SetPos(glm::vec3(-89.5f, 3.0f, -43.7f));
		spotLights[11].SetPos(glm::vec3(-91.0f, 3.0f, -42.5f));
		spotLights[12].SetPos(glm::vec3(-93.2f, 3.0f, -40.1f));
		spotLights[1].SetPos(glm::vec3(-93.2f, 3.0f, -42.0f));
		spotLights[2].SetPos(glm::vec3(-91.2f, 3.0f, -37.0f));
		spotLights[3].SetPos(glm::vec3(-85.9f, 2.0f, -41.0f));
		spotLights[4].SetPos(glm::vec3(-93.2f, 3.0f, -41.0f));
		spotLights[5].SetPos(glm::vec3(-91.6f, 3.0f, -35.9f));
		spotLights[6].SetPos(glm::vec3(-90.6f, 4.0f, -41.0f));
		spotLights[7].SetPos(glm::vec3(-90.1f, 4.0f, -39.5f));
		spotLights[8].SetPos(glm::vec3(-94.2f, 3.0f, -40.1f));
		spotLights[9].SetPos(glm::vec3(-87.9f, 3.0f, -40.2f));
		spotLights[13].SetPos(glm::vec3(-88.9f, 3.0f, -37.2f));
		spotLights[14].SetPos(glm::vec3(-92.0f, 3.0f, -37.2f));

		apagado = mainWindow.getapaga();
		if (!apagado && !dia) {

			spotLights[1].SetEdge(15);
			spotLights[2].SetEdge(15); 
			spotLights[3].SetEdge(15);
			spotLights[4].SetEdge(15);
			spotLights[5].SetEdge(15);
			spotLights[6].SetEdge(15);
			spotLights[7].SetEdge(15);
			spotLights[8].SetEdge(15);
			spotLights[9].SetEdge(15);
			spotLights[10].SetEdge(15);
			spotLights[11].SetEdge(15);
			spotLights[12].SetEdge(15);
			spotLights[13].SetEdge(15);
			spotLights[14].SetEdge(15);
		}
		else {
			spotLights[1].SetEdge(0);
			spotLights[2].SetEdge(0);
			spotLights[3].SetEdge(0);
			spotLights[4].SetEdge(0);
			spotLights[5].SetEdge(0);
			spotLights[6].SetEdge(0);
			spotLights[7].SetEdge(0);
			spotLights[8].SetEdge(0);
			spotLights[9].SetEdge(0);
			spotLights[10].SetEdge(0);
			spotLights[11].SetEdge(0);
			spotLights[12].SetEdge(0);
			spotLights[13].SetEdge(0);
			spotLights[14].SetEdge(0);
		}



		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		tempo++;
		enciende = mainWindow.getBailaLuz();
		

		//es para reiniciar los contadores si cancelamos el espectaculo 
		if (!enciende) {
			cont_espectculo_de_luces = 0;
			tempo = 0;
		}
		//inicia espectaculo de luces, esta funcion modifica las luces.
		if (enciende && !dia && !apagado) {
			if (cont_espectculo_de_luces < 25) {
				spotLights[1].SetEdge(0);
				spotLights[2].SetEdge(0);
				spotLights[3].SetEdge(0);
				spotLights[4].SetEdge(0);
				spotLights[5].SetEdge(0);
				spotLights[6].SetEdge(0);
				spotLights[7].SetEdge(0);
				spotLights[8].SetEdge(0);
				spotLights[9].SetEdge(0);
				spotLights[10].SetEdge(0);
				spotLights[11].SetEdge(0);
				spotLights[12].SetEdge(0);
				spotLights[13].SetEdge(0);
				spotLights[14].SetEdge(0);
				cont_espectculo_de_luces++;
			}
			if (cont_espectculo_de_luces >= 25 && cont_espectculo_de_luces < 50) {
				spotLights[1].SetEdge(15);
				spotLights[2].SetEdge(15);
				spotLights[3].SetEdge(15);
				spotLights[4].SetEdge(15);
				spotLights[5].SetEdge(15);
				spotLights[6].SetEdge(15);
				spotLights[7].SetEdge(15);
				spotLights[8].SetEdge(15);
				spotLights[9].SetEdge(15);
				spotLights[10].SetEdge(15);
				spotLights[11].SetEdge(15);
				spotLights[12].SetEdge(15);
				spotLights[13].SetEdge(15);
				spotLights[14].SetEdge(15);

				spotLights[10].SetPos(glm::vec3(-94.75f, 4.0f, -42.248f));
				spotLights[11].SetPos(glm::vec3(-94.75f, 4.0f, -40.248f));
				spotLights[12].SetPos(glm::vec3(-94.75f, 4.0f, -38.248f));

				spotLights[1].SetPos(glm::vec3(-86.75f, 4.0f, -38.248f));
				spotLights[2].SetPos(glm::vec3(-86.75f, 4.00f, -40.248));
				spotLights[3].SetPos(glm::vec3(-86.75f, 4.0f, -42.248f));

				spotLights[4].SetPos(glm::vec3(-92.75f, 4.0f, -36.248f));
				spotLights[5].SetPos(glm::vec3(-90.75f, 4.0f, -36.248f));
				spotLights[6].SetPos(glm::vec3(-88.75f, 4.00f, -36.248));

				spotLights[7].SetPos(glm::vec3(-90.75f, 4.0f, -44.248f));
				spotLights[8].SetPos(glm::vec3(-92.75f, 4.0f, -44.248f));
				spotLights[9].SetPos(glm::vec3(-88.75f, 4.0f, -44.248f));

				spotLights[13].SetPos(glm::vec3(-90.75f, 5.0f, -40.248f));
				spotLights[14].SetPos(glm::vec3(-90.75f, 2.0f, -40.248f));
				cont_espectculo_de_luces++;
			}
			if (cont_espectculo_de_luces >= 50) {
				spotLights[1].SetEdge(rand1);
				spotLights[2].SetEdge(rand2);
				spotLights[3].SetEdge(rand3);
				spotLights[4].SetEdge(rand4);
				spotLights[5].SetEdge(rand5);
				spotLights[6].SetEdge(rand6);
				spotLights[7].SetEdge(rand7);
				spotLights[8].SetEdge(rand8);
				spotLights[9].SetEdge(rand9);
				spotLights[10].SetEdge(rand10);
				spotLights[11].SetEdge(rand11);
				spotLights[12].SetEdge(rand12);
				spotLights[13].SetEdge(rand13);
				spotLights[14].SetEdge(rand14);
				spotLights[10].SetPos(glm::vec3(rand_pos_x1, rand_pos_y1, rand_pos_z1));
				spotLights[11].SetPos(glm::vec3(rand_pos_x2, 1 + rand_pos_y2, rand_pos_z2));
				spotLights[12].SetPos(glm::vec3(rand_pos_x3, rand_pos_y3, rand_pos_z3));
				spotLights[1].SetPos(glm::vec3(rand_pos_x4, rand_pos_y4, rand_pos_z4));
				spotLights[2].SetPos(glm::vec3(rand_pos_x5, rand_pos_y5, rand_pos_z5));
				spotLights[3].SetPos(glm::vec3(rand_pos_x6, rand_pos_y6, rand_pos_z6));
				spotLights[4].SetPos(glm::vec3(rand_pos_x7, rand_pos_y7, rand_pos_z7));
				spotLights[5].SetPos(glm::vec3(rand_pos_x8, rand_pos_y8, rand_pos_z8));
				spotLights[6].SetPos(glm::vec3(rand_pos_x9, rand_pos_y9, rand_pos_z9));
				spotLights[7].SetPos(glm::vec3(rand_pos_x10, rand_pos_y10, rand_pos_z10));
				spotLights[8].SetPos(glm::vec3(rand_pos_x11, rand_pos_y11, rand_pos_z11));
				spotLights[9].SetPos(glm::vec3(rand_pos_x12, rand_pos_y12, rand_pos_z12));
				spotLights[13].SetPos(glm::vec3(rand_pos_x13, rand_pos_y13, rand_pos_z13));
				spotLights[14].SetPos(glm::vec3((rand_pos_x14, rand_pos_y14, rand_pos_z14)));
				cont_espectculo_de_luces++;
				if (cont_espectculo_de_luces%50==0)
				{
					
					tempo = 0;
					rand1 =4+ rand() % 20;
					 rand2 = rand() % 20;
					 rand3 =4+ rand() % 20;
					 rand4 = rand() % 20;
					 rand5 =4+ rand() % 20;
					 rand6 = rand() % 20;
					 rand7 =4+ rand() % 20;
					 rand8 = rand() % 20;
					 rand9 =4+ rand() % 20;
					 rand10 = rand() % 20;
					 rand11 =4+ rand() % 20;
					 rand12 = rand() % 20;
					 rand13 =4+ rand() % 20;
					 rand14 = rand() % 20;
					 rand_pos_x1 = -95 + rand() % 9;
					 rand_pos_y1 = 1 + rand() % 5;
					 rand_pos_z1 = -44 + rand() % 9;
					 rand_pos_x2 = -95 + rand() % 9;
					 rand_pos_y2 = 1 + rand() % 5;
					 rand_pos_z2 = -44 + rand() % 9;
					 rand_pos_x3 = -95 + rand() % 9;
					 rand_pos_y3 = 1 + rand() % 5;
					 rand_pos_z3 = -44 + rand() % 9;
					 rand_pos_x4 = -95 + rand() % 9;
					 rand_pos_y4 = 1 + rand() % 5;
					 rand_pos_z4 = -44 + rand() % 9;
					 rand_pos_x5 = -95 + rand() % 9;
					 rand_pos_y5 = 1 + rand() % 5;
					 rand_pos_z5 = -44 + rand() % 9;
					 rand_pos_x6 = -95 + rand() % 9;
					 rand_pos_y6 = 1 + rand() % 5;
					 rand_pos_z6 = -44 + rand() % 9;
					 rand_pos_x7 = -95 + rand() % 9;
					 rand_pos_y7 = 1 + rand() % 5;
					 rand_pos_z7 = -44 + rand() % 9;
					 rand_pos_x8 = -95 + rand() % 9;
					 rand_pos_y8 = 1 + rand() % 5;
					 rand_pos_z8 = -44 + rand() % 9;
					 rand_pos_x9 = -95 + rand() % 9;
					 rand_pos_y9 = 1 + rand() % 5;
					 rand_pos_z9 = -44 + rand() % 9;
					 rand_pos_x10 = -95 + rand() % 9;
					 rand_pos_y10 = 1 + rand() % 5;
					 rand_pos_z10 = -44 + rand() % 9;
					 rand_pos_x11 = -95 + rand() % 9;
					 rand_pos_y11 = 1 + rand() % 5;
					 rand_pos_z11 = -44 + rand() % 9;
					 rand_pos_x12 = -95 + rand() % 9;
					 rand_pos_y12 = 1 + rand() % 5;
					 rand_pos_z12 = -44 + rand() % 9;
					 rand_pos_x13 = -95 + rand() % 9;
					 rand_pos_y13 = 1 + rand() % 5;
					 rand_pos_z13 = -44 + rand() % 9;
					 rand_pos_x14 = -95 + rand() % 9;
					 rand_pos_y14 = 1 + rand() % 5;
					 rand_pos_z14 = -44 + rand() % 9;
				}
				if (cont_espectculo_de_luces > 8000) {
					cont_espectculo_de_luces = 0;
				}
			}
		}




	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.75f,-2.0f, -40.248f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Fuente.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.75f, -2.0f, -24.248f));
		model = glm::scale(model, glm::vec3(0.28f, 0.3f, 0.28f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Baño.RenderModel();

		//Cerca
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f,0.0f,0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model=modelaux;


		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;


		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 68.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = modelaux;
		
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, -68.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		Reja.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja.RenderModel();


		//Arbustos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f,0.0f,-10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto.RenderModel();

		//raptor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, 1.0f, -25.0f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		raptor.RenderModel();

		//rex
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 70.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		rex.RenderModel();

		//rex1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 110.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		rex1.RenderModel();

		//estego
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -2.0f, 100.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		estego.RenderModel();


		//volcan
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -1.9f, 107.65f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		volcano.RenderModel();

		//piedra
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -2.0f, 250.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		p.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -2.0f, 250.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		p.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -2.0f, -40.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		p.RenderModel();



		//huesos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -2.0f, -40.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		h.RenderModel();


		//montaña
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -4.0f, 250.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		m.RenderModel();


		/*
		//Este es el helicoptero y helices 

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(0.0f, -1.44f, 4.0f);
		model = glm::translate(model, posblackhawk + glm::vec3(helicoptero_mueve_x, helicoptero_mueve_y, helicoptero_mueve_z));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, gira_eje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Helicoptero.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 1.2f));
		model = glm::rotate(model, girahelices * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Helices.RenderModel();
		model = modelaux;

		if (helicoptero_avanza) {
			girahelices += 4;
		}
		else
		{
			girahelices = 0;
		}
		*/
		//Elfo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.3f, -1.6f, -45.05f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo_pecho.RenderModel();
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 2.8f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo_cabeza.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo_brazo_derecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo_brazo_izquierdo.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.60f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo_pierna_derecha.RenderModel();
		modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Elfo_pierna_izquierda.RenderModel();
		model = modelaux;


























		//Conejo animacion compleja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(saltax-40.0f, saltay-2.0f,0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, gira_conejo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Conejo.RenderModel();

		if (contador_conejo2 < 10) {
			gira_conejo = 0;
			contador_conejo += 0.07f;
			if (contador_conejo < 5.0f) {
				saltax += 0.04f;
				saltay += 0.07f;
			}
			if (contador_conejo >= 5.0f && contador_conejo < 10.0f) {
				saltax += 0.04f;
				saltay -= 0.07f;
			}
			if (contador_conejo >= 10.0f) {
				contador_conejo2++;
				contador_conejo = 0;
			}
		}
		if (contador_conejo2>=10.0f){
			gira_conejo = 180;
			contador_conejo2 += 0.07f;
			if (contador_conejo2 >= 10.0f && contador_conejo2 < 15.0f) {
				saltax -= 0.04f;
				saltay += 0.07f;
			}
			if (contador_conejo2 >= 15.0f && contador_conejo2<20.0f) {
				saltax -= 0.04f;
				saltay -= 0.07f;
			}
			if (contador_conejo2 >= 20.0f) {
				contador_conejo2 = 10;
				contador_conejo3++;
				if (contador_conejo3 > 10) {
					contador_conejo2 = 0;
					contador_conejo3 = 0;
					contador_conejo = 0;
				}
			}
		}
		
		
		//Conejo Animacion Sencilla
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmueveluz2z(),mainWindow.getsalta() -2.0f, mainWindow.getmueveluz2x()));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Conejo.RenderModel();
		
	
		//Conejo Animacion Sencilla con sombrero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.5f, mainWindow.getsombrero()-2.0, 10.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.6f));
		//model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		sombrero.RenderModel();
		//Conejo Animacion Sencilla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, mainWindow.getescondido()-2.0, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Conejo.RenderModel();

		int detente = 0;
		//Conejo Animacion Sencilla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, - 2.0+ VuelaConejo, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, gira_conejo2*toRadians, glm::vec3(0.0f, 1.0f,0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Conejo.RenderModel();

		conejo = mainWindow.getvuelaconejo();
		if (conejo && !detente) {
			gira_conejo2 += 0.2f;
			VuelaConejo += 0.01f;
			if (VuelaConejo == 100)
			{
				detente = true;
			}
		}








		// Conejo keyframes
		model = glm::mat4(1.0);
		ConejoMove = glm::vec3(posXconejo + movConejo_x, posYconejo + movConejo_y, posZconejo + movConejo_z);
		model = glm::translate(model, ConejoMove);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, (giroConejosy)*toRadians, glm::vec3(0.0f, 1.0f, .0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Conejo.RenderModel();
		


		// terodactilo keyframes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 75.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		terodactilo.RenderModel();
































		

		/*
		//Agave 
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
		*/
		//spotLights[2].SetPos(glm::vec3(mainWindow.getmueveluz1x(), 0.0f, mainWindow.getmueveluz1z()));

		spotLights[0].SetEdge(mainWindow.getenciendeapaga());

		glDisable(GL_BLEND);
		glUseProgram(0);


		mainWindow.swapBuffers();
	}


	return 0;
}



void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 10)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;
				if (reproduciranimacion == 1) {
					reproduciranimacion = 0;
					ciclo = 0;
					habilitaranimacion = 0;
				}
			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			ciclo = 0;
			habilitaranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_M])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movConejo_x es: %f\n", movConejo_x);
			printf("movConejo_y es: %f\n", movConejo_y);
			printf("movConejo_z es: %f\n", movConejo_z);
			printf("presiona R para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_R])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movConejo_x += 2.0f;
			movConejo_z += 1.0;
			//giroConejosy += 15;
			printf("Conejo en X ahora es: %f\n", movConejo_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}




	if (keys[GLFW_KEY_Z])
	{

		camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -100.0f, 0.0f, 5.0f, 0.5f);
		



		
	}

}