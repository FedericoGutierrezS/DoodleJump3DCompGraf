#include "Vector3.h"
#include "Timer.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;




int main(int argc, char* argv[]) {
	//INICIALIZACION
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("ICG-UdelaR",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);

	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 640 / 480.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

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

	SDL_Event evento;

	float x, y, z;

	x = 0;
	y = 0;
	z = 7;
	float degrees = 0;

	GLfloat luz_posicion[4] = { 0, 0, 1, 1 };
	GLfloat luz_posicion1[4] = { 0, 0, -1, 1 };
	GLfloat colorLuz[4] = { 1, 1, 1, 1 };
	//FIN INICIALIZACION
	bool textOn = true;
	float timeStep = 0;
	float jumpSpeed = 9;
	float moveSpeed = 7;
	float camRot = 180;
	float dummyF;
	Timer *timer = new Timer();
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
		gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
		timeStep = timer->touch().delta;
		glPushMatrix();
		glLoadIdentity();
		gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);
		float posY = pos->getY();
		if (posY <= 2 && posY>=-0.1) {
			glScalef(1.0, posY*0.5, 1.0);
		}

		glScalef(0.3, 0.3, 0.3);
		//TRANSFORMACIONES LINEALES

		speed = dummy.multVecEsc(*dummy.normalize(*dir), moveSpeed);//Se normaliza la direcci�n de movimiento y se le asigna la velocidad
		if (pos->getY() >= 0) { //Se reduce la velocidad en funci�n de la gravedad si el personaje se encuentra saltando
			timeAcc += timeStep;
			speed->setY(jumpSpeed - timeAcc * gravity);
		}

		if (pos->getY() < 0) {
			pos->setY(0.0);
			dir->setY(0);
			timeAcc = 0;
		}

		if (speed->getModulo() > 0) {//Mientras la velocidad sea >0 se actualiza la posici�n del objeto
			pos = dummy.suma(*pos, *dummy.multVecEsc(*speed, timeStep));
		}

		glTranslatef(pos->getX(), pos->getY(), pos->getZ());//Se mueve el objeto


		//DIBUJAR OBJETOS
		//DIBUJO TRIANGULO CON COLOR
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(1., -1., 0.);
		glVertex3f(-1., -1., 0.);
		glVertex3f(0., 1., 0.);
		glEnd();

		glPopMatrix();
		glPushMatrix();

		//DIBUJO ESCENARIO(Con giro de camara)
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
				camRot = fmod((camRot + evento.motion.xrel*0.01),360);
				break;
			case SDL_QUIT:
				fin = true;
				break;
			case SDL_KEYDOWN:
				switch (evento.key.keysym.sym) {
				case SDLK_RIGHT:
					dir->setX(1);
					movingr = true;
					break;
				case SDLK_LEFT:
					dir->setX(-1);
					movingl = true;
					break;
				case SDLK_UP:
					dir->setZ(-1);
					movingf = true;
					break;
				case SDLK_DOWN:
					dir->setZ(1);
					movingb = true;
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
					if (!movingl) dir->setX(0);
					movingr = false;
					break;
				case SDLK_LEFT:
					if (!movingr) dir->setX(0);
					movingl = false;
					break;
				case SDLK_UP:
					if (!movingb) dir->setZ(0);
					movingf = false;
					break;
				case SDLK_DOWN:
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
