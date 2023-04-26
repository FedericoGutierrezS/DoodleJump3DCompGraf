#include "Vector3.h"
#include "Timer.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "mesh.h"
#include "plataforma.h"
#include "jugador.h"

using namespace std;

Plataforma* colision(Jugador* j, Plataforma** p, int c) {
	Plataforma* ret = NULL;
	int i = 0;
	while (i < c && ret == NULL) {
		if ((j->getPos()->getY() <= p[i]->getY() + p[i]->getProfCol())&& (j->getPos()->getY() >= p[i]->getY()+ p[i]->getProfCol()-0.2)) {
			if ((j->getPos()->getZ() - j->getAnchCol() <= p[i]->getZ() + p[i]->getAnchCol()) && (j->getPos()->getZ() + j->getAnchCol() >= p[i]->getZ() - p[i]->getAnchCol())) {
				if ((j->getPos()->getX() - j->getAltCol() <= p[i]->getX() + p[i]->getAltCol()) && (j->getPos()->getX() + j->getAltCol() >= p[i]->getX() - p[i]->getAltCol())) {
					ret = p[i];
				}
			}
		}
		i++;
	}
	return ret;
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
	int vertAmountJugador = 0;
	int vertAmountPlataforma = 0;
	Vector3** jugador = DoTheImportThing("jugador.obj", vertAmountJugador);//mesh.h
	Vector3** plataforma = DoTheImportThing("plataforma.obj", vertAmountPlataforma);


	//TEXTURA
	char* archivo = new char[20];
	archivo = "jugador.png";

	//CARGAR IMAGEN
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
	FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wj = FreeImage_GetWidth(bitmap);
	int hj = FreeImage_GetHeight(bitmap);
	void* datosJugador = FreeImage_GetBits(bitmap);

	archivo = "plataforma.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wp = FreeImage_GetWidth(bitmap);
	int hp = FreeImage_GetHeight(bitmap);
	void* datosPlataforma = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	float degrees = 0;

	GLfloat luz_posicion[4] = { 0, 3, 1, 1 };
	GLfloat colorLuz[4] = { 1, 1, 1, 1 };
	//FIN INICIALIZACION

	const float pi = 3.14159;

	bool textOn = true;
	float timeStep = 0;
	float jumpSpeed = 6;
	float moveSpeed = 4;
	float camRot = pi/2;
	float camSens = 0.004;
	float radioCamara = 7;
	Timer* timer = new Timer();

	float alturaDerrota = -0.2;

	Vector3 dummy;

	Vector3* dir = new Vector3(0, 0, 0);

	float timeAcc = 0;
	float gravity = 9.8;


	Plataforma* choque = NULL;
	Plataforma** plataformas = new Plataforma*[30];
	plataformas[0] = new Plataforma(0, 1.5, -2, 1.4, 0.5, 0.3);
	plataformas[1] = new Plataforma(2, 3, -6, 1.4, 0.5, 0.3);
	plataformas[2] = new Plataforma(-3, 4, -3, 1.4, 0.5, 0.3);
	plataformas[3] = new Plataforma(0, 5.5, 0, 1.4, 0.5, 0.3);
	plataformas[4] = new Plataforma(4, 7, 0, 1.4, 0.5, 0.3);
	int cantPlat = 5;

	Jugador* jug = new Jugador(0.3, 0.3, 0.3);
	//LOOP PRINCIPAL
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		x = radioCamara * cos(camRot);
		z = radioCamara * sin(camRot);
		if (camType) //Se elige el tipo de camara(con V)
			gluLookAt(x + jug->getPos()->getX(), 1.5 + jug->getPos()->getY(), z + jug->getPos()->getZ(), jug->getPos()->getX(), jug->getPos()->getY(), jug->getPos()->getZ(), 0, 1, 0);//Camara centrada en el jugador
		else
			gluLookAt(x, 1.5 + jug->getPos()->getY(), z, 0, jug->getPos()->getY(), 0, 0, 1, 0);//Camara centrada en el escenario

		//PRENDO LA LUZ (SIEMPRE DESPUES DEL gluLookAt)
		glEnable(GL_LIGHT0); // habilita la luz 0
		glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLuz);

		timeStep = timer->touch().delta;
		glPushMatrix();

		//TRANSFORMACIONES LINEALES
		jug->setVel(dummy.multVecEsc(*dummy.normalize(*dir), moveSpeed));//Se normaliza la dirección de movimiento y se le asigna la velocidad
		if (jug->getPos()->getY() >= 0 ) { //Se reduce la velocidad en función de la gravedad si el personaje se encuentra saltando
			timeAcc += timeStep;
			jug->getVel()->setY(jumpSpeed - timeAcc * gravity);
			choque = colision(jug, plataformas, cantPlat);
			if (choque != NULL) {
				timeAcc = 0;
				alturaDerrota = choque->getY() - 0.3;
			}
		}
		if (jug->getPos()->getY() < alturaDerrota) {
			cout << "Game Over";
			alturaDerrota = -0.2;
		}

		if (jug->getPos()->getY() < 0) {
			jug->getPos()->setY(0.0);
			dir->setY(0);
			timeAcc = 0;
		}

		if (jug->getVel()->getModulo() > 0) {//Mientras la velocidad sea >0 se actualiza la posición del objeto
			jug->setPos(dummy.suma(*jug->getPos(), *dummy.multVecEsc(*jug->getVel(), timeStep)));
		}
		
		//DIBUJAR OBJETOS
		//DIBUJO MODELO
		glEnable(GL_LIGHTING);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wj, hj, 0, GL_BGR, GL_UNSIGNED_BYTE, datosJugador);
		jug->draw(jugador, vertAmountJugador, textura);
		glDisable(GL_LIGHTING);
		glPopMatrix();
		//DIBUJO ESCENARIO(Sin movimiento de personaje)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
		for (int i = 0; i < cantPlat; i++) {
			plataformas[i]->draw(plataforma, vertAmountPlataforma, textura);
		}
		//FIN DIBUJAR OBJETOS

		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)) {
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEWHEEL:
				radioCamara = radioCamara - evento.wheel.y*0.5;
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

						glBindTexture(GL_TEXTURE_2D, textura);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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

						glBindTexture(GL_TEXTURE_2D, textura);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
