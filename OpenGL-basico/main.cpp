#include "Vector3.h"
#include "Timer.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>

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
	bool rotate = false;

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
	float initialTime = 0;
	float endTime = 0;
	//LOOP PRINCIPAL
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
		endTime = SDL_GetTicks();
		float timeStep = endTime - initialTime;
		initialTime = endTime;
		//PRENDO LA LUZ (SIEMPRE DESPUES DEL gluLookAt)
		glEnable(GL_LIGHT0); // habilita la luz 0
		glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLuz);

		glEnable(GL_LIGHT1); // habilita la luz 1
		glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, colorLuz);

		glPushMatrix();
		//TRANSFORMACIONES LINEALES
		if (rotate) {
			degrees = degrees + 1;
			cout << 1000 / timeStep;
			cout << "  ";
		}
		glRotatef(degrees, 0.0, 1.0, 0.0);

		//DIBUJAR OBJETOS
		//DIBUJO TRIANGULO CON COLOR
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(1., -1., 0.);
		glVertex3f(-1., -1., 0.);
		glVertex3f(0., 1., 0.);
		glEnd();
		glPopMatrix();

		//DIBUJO TRIANGULO CON TEXTURA
		if (textOn) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textura);
		}
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0);
		glVertex3f(3., -1., 0.);
		glTexCoord2f(0, 1);
		glVertex3f(1., -1., 0.);
		glTexCoord2f(1, 0);
		glVertex3f(2., 1., 0.);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//DIBUJO TRIANGULO CON LUZ
		glEnable(GL_LIGHTING);
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, 1);
		glVertex3f(-1., -1., 0.);
		glVertex3f(-3., -1., 0.);
		glVertex3f(-2., 1., 0.);
		glEnd();
		glDisable(GL_LIGHTING);


		//FIN DIBUJAR OBJETOS

		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)) {
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				rotate = true;
				cout << "ROT\n";
				break;
			case SDL_MOUSEBUTTONUP:
				rotate = false;
				break;
			case SDL_QUIT:
				fin = true;
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
	return 0;
}
