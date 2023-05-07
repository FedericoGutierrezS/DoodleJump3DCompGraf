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
#include "enemigo.h"
#include "jetpack.h"
#include "HUD.h"
#include "bullet.h"
#include <algorithm>
#include <stdlib.h>
#include <string>

using namespace std;

void generate_object(string seed, float height,int &xcoord,int &ycoord) {
	// Use the random number and the seed to generate a hash.
	size_t hash_value = hash<string>{}(to_string((int)height) + seed);
	// Convert the hash value to a string.
	string hash_string = to_string(hash_value);
	// Split the hash string into two parts, each of which is 16 characters long.
	string left_part = hash_string.substr(0, 5);
	string right_part = hash_string.substr(5);
	// Use the left and right parts of the hash string to generate the x and y coordinates of the object.
	int x_coordinate = stoi(left_part,NULL,10);
	int y_coordinate = stoi(right_part,NULL,10);

	// Return the object at the specified coordinates.
	xcoord = x_coordinate;
	ycoord = y_coordinate;
}

Plataforma* colision(Jugador* j, Plataforma** p, int c) {
	Plataforma* ret = NULL;
	int i = 0;
	while (i < c && ret == NULL) {
		if (p[i]->getExists()) {
			if ((j->getPos()->getY() <= p[i]->getY() + p[i]->getProfCol()) && (j->getPos()->getY() >= p[i]->getY())) {
				if ((j->getPos()->getZ() - j->getAnchCol() <= p[i]->getZ() + p[i]->getAnchCol()) && (j->getPos()->getZ() + j->getAnchCol() >= p[i]->getZ() - p[i]->getAnchCol())) {
					if ((j->getPos()->getX() - j->getAltCol() <= p[i]->getX() + p[i]->getAltCol()) && (j->getPos()->getX() + j->getAltCol() >= p[i]->getX() - p[i]->getAltCol())) {
						ret = p[i];
					}
				}
			}
		}
		i++;
	}
	return ret;
}

Enemigo* colision(Jugador* j, Enemigo** p, int c) {
	Enemigo* ret = NULL;
	int i = 0;
	while (i < c && ret == NULL) {
		if (p[i]->getExists()) {
			if ((j->getPos()->getY() <= p[i]->getPos()->getY() + p[i]->getProfCol()) && (j->getPos()->getY() + j->getProfCol() >= p[i]->getPos()->getY())) {
				if ((j->getPos()->getZ() - j->getAnchCol() <= p[i]->getPos()->getZ() + p[i]->getAnchCol()) && (j->getPos()->getZ() + j->getAnchCol() >= p[i]->getPos()->getZ() - p[i]->getAnchCol())) {
					if ((j->getPos()->getX() - j->getAltCol() <= p[i]->getPos()->getX() + p[i]->getAltCol()) && (j->getPos()->getX() + j->getAltCol() >= p[i]->getPos()->getX() - p[i]->getAltCol())) {
						ret = p[i];
					}
				}
			}
		}
		i++;
	}
	return ret;
}

Enemigo* colision(Bullet* b, Enemigo** p, int c) {
	Enemigo* ret = NULL;
	int i = 0;
	while (i < c && ret == NULL) {
		if (p[i]->getExists()) {
			if ((b->getPos()->getY() <= p[i]->getPos()->getY() + p[i]->getProfCol()) && (b->getPos()->getY() + b->getProfCol() >= p[i]->getPos()->getY())) {
				if ((b->getPos()->getZ() - b->getAnchCol() <= p[i]->getPos()->getZ() + p[i]->getAnchCol()) && (b->getPos()->getZ() + b->getAnchCol() >= p[i]->getPos()->getZ() - p[i]->getAnchCol())) {
					if ((b->getPos()->getX() - b->getAltCol() <= p[i]->getPos()->getX() + p[i]->getAltCol()) && (b->getPos()->getX() + b->getAltCol() >= p[i]->getPos()->getX() - p[i]->getAltCol())) {
						ret = p[i];
					}
				}
			}
		}
		i++;
	}
	return ret;
}

bool colision(Jugador* j, Jetpack* jp) {
	bool res = false;
	if (jp->getExist()) {
		if ((j->getPos()->getY() <= jp->getPos()->getY() + jp->getProfCol()) && (j->getPos()->getY() >= jp->getPos()->getY())) {
			if ((j->getPos()->getZ() - j->getAnchCol() <= jp->getPos()->getZ() + jp->getAnchCol()) && (j->getPos()->getZ() + j->getAnchCol() >= jp->getPos()->getZ() - jp->getAnchCol())) {
				if ((j->getPos()->getX() - j->getAltCol() <= jp->getPos()->getX() + jp->getAltCol()) && (j->getPos()->getX() + j->getAltCol() >= jp->getPos()->getX() - jp->getAltCol())) {
					res = true;
					;
				}
			}
		}
	}
	return res;
}

void makeObjectLookAtMovementDir(Vector3* dir) {
	if (dir->getX() == -1) {
		if (dir->getZ() == 0) glRotatef(180, 0, 1, 0);
		else if (dir->getZ() == -1) glRotatef(135, 0, 1, 0);
		else glRotatef(225, 0, 1, 0);
	}
	else if (dir->getX() == 1) {
		if (dir->getZ() == 0);
		else if (dir->getZ() == -1) glRotatef(45, 0, 1, 0);
		else glRotatef(315, 0, 1, 0);
	}
	else if (dir->getX() == 0 && dir->getZ() == 1) glRotatef(270, 0, 1, 0);
	else if (dir->getX() == 0 && dir->getZ() == -1) glRotatef(90, 0, 1, 0);
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
	int vertAmountEnemigo = 0;
	int vertAmountBala = 0;
	int vertAmountJetpack = 0;
	Vector3** jugador = DoTheImportThing("jugador.obj", vertAmountJugador);//mesh.h
	Vector3** plataforma = DoTheImportThing("plataforma.obj", vertAmountPlataforma);
	Vector3** enemigo1 = DoTheImportThing("enemigo.obj", vertAmountEnemigo);
	Vector3** bala = DoTheImportThing("bala.obj", vertAmountBala);
	Vector3** jetpack = DoTheImportThing("jugador.obj", vertAmountJetpack);

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

	archivo = "atlasFont.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wa = FreeImage_GetWidth(bitmap);
	int ha = FreeImage_GetHeight(bitmap);
	void* datosAtlasFont = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "Enemigo.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int we = FreeImage_GetWidth(bitmap);
	int he = FreeImage_GetHeight(bitmap);
	void* datosEnemigo = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "Bala.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wb = FreeImage_GetWidth(bitmap);
	int hb = FreeImage_GetHeight(bitmap);
	void* datosBala = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "white_square.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wje = FreeImage_GetWidth(bitmap);
	int hje = FreeImage_GetHeight(bitmap);
	void* datosJetpack = FreeImage_GetBits(bitmap);
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
	bool camType = false;//Tipo de camara(Se cambia con V)
	bool fullscreen = false;
	bool pause = false;
	bool texturas = true;
	bool wireframe = false;
	bool facetado = false;
	bool enemyDir = true;//Usado para el cambio de direccion del enemigo

	SDL_Event evento;

	float x, y, z;
	int yAnt = 0;

	float degrees = 0;

	GLfloat luz_posicion[4] = { 0, 3, 1, 1 };
	GLfloat luz_posicion1[4] = { 0, 3, 1, 1 };
	GLfloat colorLuz[4] = { 1, 1, 1, 1 };
	//FIN INICIALIZACION

	const float pi = 3.14159;

	float timeStep = 0;
	float jumpSpeed = 6;//Velocidad inicial del salto del jugador
	float moveSpeed = 5;//Velocidad del jugador
	float enemigo1Speed = 4;//Velocidad del enemigo(Posiblemente puede convenir meterla dentro de la clase enemigo
	float camRot = pi/2;//Angulo actual de la camara
	float camSens = 0.004;//Sensibilidad de la camara
	float radioCamara = 7;//Radio de la camara, se ajusta en juego con la ruedita
	float viewDistance = 5;//Radio alrededor del personaje para el cual se renderizan las plataformas
	float bulletSpeed = 8;
	float jetpackTime = 5;//Duracion del jetpack

	Timer* timer = new Timer();//timer para el timeStep
	Timer* jetpackTimer = new Timer();//controla en tiempo que el jugador tiene el jetpack puesto
	Timer* jetpackRemovalTimer = new Timer();//controla el timepo luego de que se le termina el jetpack al jugador
	float jetpackElapsedTime = -1;
	float jetpackRemovedElapsedTime = -1;
	float jetpackTimeToGetToFullGravity = 5;

	float alturaDerrota = -200;

	Vector3 dummy;//Auxiliar para usar operaciones de Vector3

	Vector3* dir = new Vector3(0, 0, 0);//Direccion a la que se tiene que mover el personaje(WASD o FLECHAS)
	Vector3* direccionBala = new Vector3(0, 0, 0);

	float timeAcc = 0;
	float gravity = 11;
	float velocidadJuego = 1;
	float tiempoTranscurrido = 0;
	float score = 0;
	float altAlcanzada = 0;
	string seed = "sda";

	//Generacion de plataformas
	Plataforma* choque = NULL;
	Plataforma** plataformas = new Plataforma*[11];
	int cantPlat = 11;
	//Generacion de enemigos
	Enemigo* colEnemigo = NULL;
	Enemigo* enemigoHerido = NULL;
	Enemigo** enemigos = new Enemigo*[10];
	enemigos[0] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[0]->setPos(2, 3.4, -6);
	enemigos[1] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[1]->setPos(4, 7.4, 0);
	enemigos[2] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[2]->setPos(1, 21.9, 1);
	enemigos[3] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[3]->setPos(0, 27.4, 1);
	int cantEnem = 4;
	//Generacion de jetpack
	bool colJetpack = false;
	Jetpack* jetp = new Jetpack(1, 1, 1);
	
	jetp->setPos(0., 0., -5.);

	//Se crea el jugador
	Jugador* jug = new Jugador(0.3, 0.3, 0.2);
	//Se crea la bala
	Bullet* bul = new Bullet(0, 0, 0, 0.1, 0.1, 0.5);
	
	//LOOP PRINCIPAL

	for (int i = 0; i < 11; i++) {
		int xcoord = 2, zcoord = 0;
		generate_object(seed, i, xcoord, zcoord);
		xcoord = (xcoord % 100) * 0.06;
		zcoord = (zcoord % 100) * 0.06;
		plataformas[i % 11] = new Plataforma(xcoord, i, zcoord, 1.4, 0.5, 0.3, 'n');
	}
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		x = radioCamara * cos(camRot);
		z = radioCamara * sin(camRot);
		if (camType) //Se elige el tipo de camara(con V)
			gluLookAt(x + jug->getPos()->getX(), 1.5 + jug->getPos()->getY(), z + jug->getPos()->getZ(), jug->getPos()->getX(), jug->getPos()->getY(), jug->getPos()->getZ(), 0, 1, 0);//Camara centrada en el jugador
		else
			gluLookAt(jug->getPos()->getX()+x*1/radioCamara,jug->getPos()->getY()+0.5, jug->getPos()->getZ()+z*1/radioCamara, jug->getPos()->getX(), jug->getPos()->getY()+0.5, jug->getPos()->getZ()+0.5, 0, 1, 0);//Camara centrada en el escenario

		//PRENDO LA LUZ (SIEMPRE DESPUES DEL gluLookAt)
		glEnable(GL_LIGHT0); // habilita la luz 0
		glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLuz);
		glEnable(GL_LIGHT1); // habilita la luz 1
		glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, colorLuz);

		timeStep = velocidadJuego*timer->touch().delta;
		if (pause) timeStep = 0;//Pausa
		tiempoTranscurrido = tiempoTranscurrido + timeStep;//Contador de tiempo
		
		if(jug->getPos()->getY() > 5)
			for (int i = jug->getPos()->getY() - 5; i < jug->getPos()->getY() + 5; i++) {
				int xcoord = 2, zcoord = 0;
				generate_object(seed, i, xcoord, zcoord);
				xcoord = (xcoord % 100) * 0.06;
				zcoord = (zcoord % 100) * 0.06;
				plataformas[i % 11]->setPos(xcoord, i, zcoord);
			}
		//TRANSFORMACIONES LINEALES
		jug->setVel(dummy.multVecEsc(*dummy.normalize(*dir), moveSpeed));//Se normaliza la dirección de movimiento y se le asigna la velocidad
		if (jug->getPos()->getY() >= 0 ) { //Se reduce la velocidad en función de la gravedad si el personaje se encuentra saltando
			timeAcc += timeStep;
			jug->getVel()->setY(jumpSpeed - timeAcc * gravity);
			choque = colision(jug, plataformas, cantPlat);//Chequeo de colision con plataformas
			colEnemigo = colision(jug, enemigos, cantEnem);//Chequeo de colision con enemigos
			enemigoHerido = colision(bul, enemigos, cantEnem);//Chequeo de colision de bala con enemigo
			colJetpack = colision(jug, jetp);//
			if (colJetpack) {
				//Si colisiono con el jetpack me lo pongo y arranco el timer
				jetp->setOnPlayer(true);
				if (jetpackElapsedTime == -1) {
					jetpackTimer->peek();
					jetpackElapsedTime = 0;
				}
					
			}
			//Si choca con una plataforma, salta nuevamente(acumulador de tiempo de la ecuacion vuelve a 0) y ademas actualiza tanto altura de derrota como score, yAnt es usado en la animacion de salto
			if (choque != NULL) {
				timeAcc = 0;
				if(alturaDerrota < choque->getY() - 2) alturaDerrota = choque->getY() - 2;
				yAnt = choque->getY();
				if (choque->getType() == 'd') choque->setExists(false);
				if (alturaDerrota > altAlcanzada) {
					score = score + (alturaDerrota - altAlcanzada) * 300;
					altAlcanzada = alturaDerrota;
				}
				choque = NULL;
			}
		}

		if (enemigoHerido != NULL) {//Si herimos a un enemigo, este desaparece
			enemigoHerido->setExists(false);
			bul->setExists(false);
			score = score + 371;
			enemigoHerido = NULL;
		}

		//Condiciones de Derrota
		if (jug->getPos()->getY() < alturaDerrota || colEnemigo!=NULL) {
			alturaDerrota = -200;
			tiempoTranscurrido = 0;
			score = 0;
			jug->setPos(0,0,0);
			yAnt = 0;
			altAlcanzada = 0;
			//Se marcan todas las plataformas como existentes nuevamente
			for (int i = 0; i < 11; i++) {
				int xcoord = 2, zcoord = 0;
				generate_object(seed, i, xcoord, zcoord);
				xcoord = (xcoord % 100) * 0.06;
				zcoord = (zcoord % 100) * 0.06;
				plataformas[i % 11]->setPos(xcoord,i,zcoord);
			}
			for (int i = 0; i < cantPlat; i++) {
				plataformas[i]->setExists(true);
			}
			for (int i = 0; i < cantEnem; i++) {
				enemigos[i]->setExists(true);
			}
			bul->getPos()->setY(0);
		}
		//La altura 0 es piso
		if (jug->getPos()->getY() < 0) {
			jug->getPos()->setY(0.0);
			dir->setY(0);
			timeAcc = 0;
			yAnt = 0;
		}

		if (jug->getVel()->getModulo() > 0) {//Mientras la velocidad sea >0 se actualiza la posición del objeto
			jug->setPos(dummy.suma(*jug->getPos(), *dummy.multVecEsc(*jug->getVel(), timeStep)));
		}
		
		//Movimiento de la bala
		if (bul->getExists()) {
			bul->setPos(dummy.suma(*bul->getPos(), *dummy.multVecEsc(*dummy.multVecEsc(*dummy.normalize(*direccionBala), bulletSpeed), timeStep)));
			if (dummy.resta(*jug->getPos(), *bul->getPos())->getModulo() > 10)
				bul->setExists(false);
		};

		//Opciones necesarias
		if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (texturas) glEnable(GL_TEXTURE_2D);
		if (facetado) glShadeModel(GL_FLAT);
		else glShadeModel(GL_SMOOTH);
		//Luces alrededor del jugador
		luz_posicion[0] = jug->getPos()->getX() + 2;
		luz_posicion[1] = jug->getPos()->getY() + 1;
		luz_posicion[2] = jug->getPos()->getZ() + 2;
		luz_posicion1[0] = jug->getPos()->getX() - 2;
		luz_posicion1[1] = jug->getPos()->getY() + 1;
		luz_posicion1[2] = jug->getPos()->getZ() - 2;
		glEnable(GL_LIGHTING);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wj, hj, 0, GL_BGR, GL_UNSIGNED_BYTE, datosJugador);
		glPushMatrix();
		//Se translada al personaje
		glTranslatef(jug->getPos()->getX(), jug->getPos()->getY(), jug->getPos()->getZ());
		//Se gira al personaje para que mire a donde apunta la direccion de movimiento
		makeObjectLookAtMovementDir(dir);
		//Animacion salto personaje
		glScalef(1, min(max(jug->getPos()->getY() - yAnt,0.2f)*0.5f,1.0f), 1);
		//Dibujado de personaje
		if(camType) jug->draw(jugador, vertAmountJugador, textura);
		glDisable(GL_LIGHTING);
		glPopMatrix();

		//Dibujado de bala
		if (bul->getExists()) {
			glPushMatrix();
			glTranslatef(bul->getPos()->getX(), bul->getPos()->getY()+0.8f, bul->getPos()->getZ());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wb, hb, 0, GL_BGR, GL_UNSIGNED_BYTE, datosBala);
			bul->draw(bala, vertAmountBala, textura);
			glPopMatrix();
		}

		//Checkeo timer del jetpack
		if (jetp->getOnPlayer()) {
			jetpackElapsedTime += jetpackTimer->touch().delta;
			gravity = -0.1;
			if (jetpackElapsedTime >= jetpackTime) {
				//Se acabo el tiempo del jetpack
				jetpackElapsedTime = -1;
				jetp->setOnPlayer(false);
				jetp->setExist(false);
				jetpackRemovalTimer->peek();
				jetpackRemovedElapsedTime = 0;
			}
		}
		else {
			if (jetpackRemovedElapsedTime != -1) {
				//segundos luego de que se acabe el jetpack tengo que aumentar la gravedad gradualmente.
				jetpackRemovedElapsedTime += jetpackRemovalTimer->touch().delta;
				if (jetpackRemovedElapsedTime <= jetpackTimeToGetToFullGravity) {
					float gravityMultiplier = jetpackRemovedElapsedTime / jetpackTimeToGetToFullGravity;
					gravity = 11 * gravityMultiplier;
				}
				else
					jetpackRemovedElapsedTime = -1;
			}
			else {
				gravity = 11;
			}
			
		}

		//Dibujado del jetpack
		if (jetp->getExist()) {
			glPushMatrix();
			glTranslatef(jetp->getPos()->getX(), jetp->getPos()->getY() + 0.8f, jetp->getPos()->getZ());
			if (jetp->getOnPlayer()) {
				jetp->setPos(jug->getPos()->getX() + 0.2, jug->getPos()->getY() - 0.3, jug->getPos()->getZ() + 0.2);
				makeObjectLookAtMovementDir(dir);
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wb, hb, 0, GL_BGR, GL_UNSIGNED_BYTE, datosJetpack);
			jetp->draw(jetpack, vertAmountJetpack, textura);
			glPopMatrix();
		}
		
	
		//DIBUJO ESCENARIO(Sin movimiento de personaje)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
		for (int i = 0; i < cantPlat; i++) {
			if ((plataformas[i]->getY() > jug->getPos()->getY() - viewDistance) && (plataformas[i]->getY() < jug->getPos()->getY() + viewDistance) && plataformas[i]->getExists()) {
				switch (plataformas[i]->getType()) {
				case 'n':
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
					plataformas[i]->draw(plataforma, vertAmountPlataforma, textura);
					break;
				case 'd':
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
					glColor3f(1, 0, 0.412);
					plataformas[i]->draw(plataforma, vertAmountPlataforma, textura);
					glColor3f(1, 1, 1);
					break;
				}
			}
		}
		//Movimiento de los enemigos oscilante sobre sus plataformas y renderizado
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, we, he, 0, GL_BGR, GL_UNSIGNED_BYTE, datosEnemigo);
		for (int i = 0; i < cantEnem; i++) {
			glPushMatrix();
			glTranslatef(enemigos[i]->getPos()->getX(), enemigos[i]->getPos()->getY(), enemigos[i]->getPos()->getZ());
			if (enemyDir && enemigos[i]->getPos()->getX() < enemigos[i]->getEnemyCenter() + 1.8) {
				enemigos[i]->getPos()->setX(enemigos[i]->getPos()->getX() + timeStep * enemigo1Speed);
				glRotatef(180, 0, 1, 0);
			}
			else if (enemigos[i]->getPos()->getX() >= enemigos[i]->getEnemyCenter() + 1.8) {
				enemyDir = !enemyDir;
				enemigos[i]->getPos()->setX(enemigos[i]->getEnemyCenter() + 1.79);
			}
			if (!enemyDir && enemigos[i]->getPos()->getX() > enemigos[i]->getEnemyCenter() - 1.8) {
				enemigos[i]->getPos()->setX(enemigos[i]->getPos()->getX() - timeStep * enemigo1Speed);
			}
			else if (enemigos[i]->getPos()->getX() <= enemigos[i]->getEnemyCenter() - 1.8) {
				enemyDir = !enemyDir;
				enemigos[i]->getPos()->setX(enemigos[i]->getEnemyCenter() - 1.79);
			}
			if (enemigos[i]->getExists()) {
				enemigos[i]->draw(enemigo1, vertAmountEnemigo, textura);
			}
			glPopMatrix();
		}
		//FIN DIBUJAR OBJETOS
		

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, 1280, 720, 0.0, -1.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);

		glClear(GL_DEPTH_BUFFER_BIT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wa, ha, 0, GL_BGR, GL_UNSIGNED_BYTE, datosAtlasFont);
		renderTime(tiempoTranscurrido, textura);
		renderScore(score, textura);
		// Making sure we can render 3d again
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_TEXTURE_2D);
		
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
				case SDLK_SPACE:
					if (!bul->getExists()) {
						bul->setExists(true);
						bul->getPos()->setX(jug->getPos()->getX());
						bul->getPos()->setY(jug->getPos()->getY());
						bul->getPos()->setZ(jug->getPos()->getZ());
						direccionBala->setX(dir->getX());
						direccionBala->setY(dir->getY());
						direccionBala->setZ(dir->getZ());
						if(direccionBala->getModulo() == 0) direccionBala->setX(1);
					}
					break;
				case SDLK_F11:
					if(!fullscreen){

						SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
						fullscreen = !fullscreen;
					}
					else{

						SDL_SetWindowFullscreen(win, 0);
						fullscreen = !fullscreen;
					}
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evento.key.keysym.sym) {
				case SDLK_q:
					fin = true;
					break;
				case SDLK_p:
					pause = !pause;
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
