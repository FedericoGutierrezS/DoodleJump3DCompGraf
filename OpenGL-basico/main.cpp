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

	x = 0;
	y = 3;
	z = 7;
	float degrees = 0;

	GLfloat luz_posicion[4] = { 0, 3, 1, 1 };
	GLfloat colorLuz[4] = { 1, 1, 1, 1 };
	//FIN INICIALIZACION

	const float pi = 3.14159;

	bool textOn = true;
	float timeStep = 0;
	float jumpSpeed = 9;
	float moveSpeed = 7;
	float camRot = pi / 2;
	float camSens = 0.004;
	float radioCamara = 7;
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
		x = radioCamara * cos(camRot);
		z = radioCamara * sin(camRot);
		if (camType) //Se elige el tipo de camara(con V)
			gluLookAt(x/3 + pos->getX() * 0.3, 1.5 + pos->getY() * 0.3, z/3 + pos->getZ() * 0.3, pos->getX() * 0.3, pos->getY() * 0.3, pos->getZ() * 0.3, 0, 1, 0);//Camara centrada en el jugador
		else
			gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);//Camara centrada en el escenario

		//PRENDO LA LUZ (SIEMPRE DESPUES DEL gluLookAt)
		glEnable(GL_LIGHT0); // habilita la luz 0
		glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLuz);

		timeStep = timer->touch().delta;
		glPushMatrix();
		float posY = pos->getY();
		if (posY <= 2 && posY >= -0.2) {
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
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wj, hj, 0, GL_BGR, GL_UNSIGNED_BYTE, datosJugador);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		drawFaces(jugador, vertAmountJugador, textura);//mesh.h
		glDisable(GL_LIGHTING);
		glPopMatrix();
		glPushMatrix();

		//DIBUJO ESCENARIO(Sin movimiento de personaje)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
		drawFaces(plataforma, vertAmountPlataforma, textura);

		glPopMatrix();
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
