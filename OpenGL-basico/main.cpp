#include "Vector3.h"
#include "Timer.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace std;


Vector3* DoTheImportThing(const std::string& pFile, int& vertAmount) {
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType);
	// If the import failed, report it

	// Now we can access the file's contents.

	// We're done. Everything will be cleaned up by the importer destructor
		
	vertAmount = scene->mMeshes[0]->mNumVertices;
	Vector3* vertices = new Vector3[vertAmount];
	for (int i = 0; i < vertAmount; i++) {
		vertices[i].setX(scene->mMeshes[0]->mVertices[i].x);
		vertices[i].setY(scene->mMeshes[0]->mVertices[i].y);
		vertices[i].setZ(scene->mMeshes[0]->mVertices[i].z);
	}
	return vertices;
};

void drawFaces(Vector3* vertices,int vertAmount) {
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glTranslatef(0, 0, -1);
	for (int i = 0; i < vertAmount; i = i + 4) {
		glBegin(GL_QUADS);
		glColor3f(0.4, fmod(i,0.3), 0.2);
		glVertex3f(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		glVertex3f(vertices[i + 1].getX(), vertices[i + 1].getY(), vertices[i + 1].getZ());
		glVertex3f(vertices[i + 2].getX(), vertices[i + 2].getY(), vertices[i + 2].getZ());
		glVertex3f(vertices[i + 3].getX(), vertices[i + 3].getY(), vertices[i + 3].getZ());
		glEnd();
	}
	glPopMatrix();
}

int main(int argc, char* argv[]) {
	//INICIALIZACION
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("ICG-UdelaR",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 1280 / 720.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	int vertAmount = 0;
	Vector3* modelo = DoTheImportThing("untitled.blend", vertAmount);


	//TEXTURA
	char* archivo = new char[20];
	archivo = "../canon.png";

	//CARGAR IMAGEN
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
	FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int w = FreeImage_GetWidth(bitmap);
	int h = FreeImage_GetHeight(bitmap);
	void* datos = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//FIN TEXTURA


	bool fin = false;
	bool movingr = false;
	bool movingl = false;
	bool movingf = false;
	bool movingb = false;
	bool camType = false;
	bool fullscreen = false;

	SDL_Event evento;

	float x, y, z;

	x = 0;
	y = 3;
	z = 7;
	float degrees = 0;

	GLfloat luz_posicion[4] = { 0, 0, 1, 1 };
	GLfloat luz_posicion1[4] = { 0, 0, -1, 1 };
	GLfloat colorLuz[4] = { 1, 1, 1, 1 };
	//FIN INICIALIZACION

	const float pi = 3.14159;

	bool textOn = true;
	float timeStep = 0;
	float jumpSpeed = 9;
	float moveSpeed = 7;
	float camRot = pi / 2;
	float camSens = 0.004;
	Timer* timer = new Timer();
	Vector3 dummy;
	Vector3* pos = new Vector3(0, 0, 0);
	Vector3* dir = new Vector3(0, 0, 0);
	Vector3* speed = new Vector3(0, 0, 0);
	float timeAcc = 0;
	float gravity = 9.8;
	//LOOP PRINCIPAL
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		x = 7 * cos(camRot);
		z = 7 * sin(camRot);
		if (camType) //Se elige el tipo de camara(con V)
			gluLookAt(x/3 + pos->getX() * 0.3,  pos->getY() * 0.3, z/3 + pos->getZ() * 0.3, pos->getX() * 0.3, pos->getY() * 0.3, pos->getZ() * 0.3, 0, 1, 0);//Camara centrada en el jugador
		else
			gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);//Camara centrada en el escenario
		timeStep = timer->touch().delta;
		glPushMatrix();
		float posY = pos->getY();
		if (posY <= 2 && posY >= -0.1) {
			glScalef(1.0, posY * 0.5, 1.0);
		}

		glScalef(0.3, 0.3, 0.3);
		//TRANSFORMACIONES LINEALES

		speed = dummy.multVecEsc(*dummy.normalize(*dir), moveSpeed);//Se normaliza la dirección de movimiento y se le asigna la velocidad
		if (pos->getY() >= 0) { //Se reduce la velocidad en función de la gravedad si el personaje se encuentra saltando
			timeAcc += timeStep;
			speed->setY(jumpSpeed - timeAcc * gravity);
		}

		if (pos->getY() < 0) {
			pos->setY(0.0);
			dir->setY(0);
			timeAcc = 0;
		}

		if (speed->getModulo() > 0) {//Mientras la velocidad sea >0 se actualiza la posición del objeto
			pos = dummy.suma(*pos, *dummy.multVecEsc(*speed, timeStep));
		}

		glTranslatef(pos->getX(), pos->getY(), pos->getZ());//Se mueve el objeto


		//DIBUJAR OBJETOS
		//DIBUJO MODELO
		drawFaces(modelo, vertAmount);
		glPopMatrix();
		glPushMatrix();

		//DIBUJO ESCENARIO(Sin movimiento de personaje)
		glTranslatef(0, -0.04, 0);
		glBegin(GL_QUADS);
		glColor3f(0.4, 0.0, 0.7);
		glVertex3f(-3., 0., 3.);
		glVertex3f(-3., 0., -3.);
		glVertex3f(3., 0., -3.);
		glVertex3f(3., 0., 3.);
		glEnd();

		glPopMatrix();

		//FIN DIBUJAR OBJETOS

		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)) {
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEMOTION:
				camRot = fmod((camRot + evento.motion.xrel * camSens), pi * 2);
				break;
			case SDL_QUIT:
				fin = true;
				break;
			case SDL_KEYDOWN:
				switch (evento.key.keysym.sym) {
				case SDLK_RIGHT:
				case SDLK_d:
					dir->setX(1);
					movingr = true;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					dir->setX(-1);
					movingl = true;
					break;
				case SDLK_UP:
				case SDLK_w:
					dir->setZ(-1);
					movingf = true;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					dir->setZ(1);
					movingb = true;
					break;
				case SDLK_v:
					camType = !camType;
					break;
				case SDLK_F11:
					if(!fullscreen){
						SDL_DestroyWindow(win);//Se destruye la ventana y se crea una nueva con la nueva resolución, creando un contexto para esta
						win = SDL_CreateWindow("ICG-UdelaR",
							SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED,
							1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
						SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
						context = SDL_GL_CreateContext(win);
						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();
						float color = 0;
						glClearColor(color, color, color, 1);
						gluPerspective(45, 1920 / 1080.f, 0.1, 100);
						glEnable(GL_DEPTH_TEST);
						glMatrixMode(GL_MODELVIEW);
						fullscreen = !fullscreen;
					}
					else{
						SDL_DestroyWindow(win);
						win = SDL_CreateWindow("ICG-UdelaR",
							SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED,
							1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
						SDL_SetWindowFullscreen(win, 0);
						context = SDL_GL_CreateContext(win);
						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();
						float color = 0;
						glClearColor(color, color, color, 1);

						gluPerspective(45, 1280 / 720.f, 0.1, 100);
						glEnable(GL_DEPTH_TEST);
						glMatrixMode(GL_MODELVIEW);
						fullscreen = !fullscreen;
					}
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evento.key.keysym.sym) {
				case SDLK_ESCAPE:
					fin = true;
					break;
				case SDLK_l:
					textOn = !textOn;
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					if (!movingl) dir->setX(0);
					movingr = false;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					if (!movingr) dir->setX(0);
					movingl = false;
					break;
				case SDLK_UP:
				case SDLK_w:
					if (!movingb) dir->setZ(0);
					movingf = false;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					if (!movingf) dir->setZ(0);
					movingb = false;
					break;
				}
			}
		}
		//FIN MANEJO DE EVENTOS
		SDL_GL_SwapWindow(win);
	} while (!fin);
	//FIN LOOP PRINCIPAL
	// LIMPIEZA
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	delete timer;
	return 0;
}
