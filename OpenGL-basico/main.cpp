#include "Vector3.h"
#include "Timer.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
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
#include "shield.h"
#include "HUD.h"
#include "bullet.h"
#include <algorithm>
#include <stdlib.h>
#include <string>

using namespace std;

//Indica a otros Threads si se esta jugando
bool playing = true;

//Cantidad maxima de pistas de audio, debe ser mayor o igual a la cantidad de audios que tiene el juego
const int pistasAudio = 2;

struct AudioData {
	Mix_Chunk* chunk;
	bool* boolean_play;
	const char* file;
	int* channel;
};

int AudioThread(void* data) {
	AudioData* channels = static_cast<AudioData*>(data);
	while (playing) {
		for (int i = 0; i < pistasAudio; i++) {
			if (*channels[i].boolean_play && (*channels[i].channel == -1 || !Mix_Playing(*channels[i].channel))) {
				int channel = Mix_PlayChannel(-1, channels[i].chunk, 0);
				if (channel == -1) {
					cerr << "No se pudo reproducir el canal de audio: " << Mix_GetError() << endl;
					exit(1);
				}
				*channels[i].channel = channel;
			}
		}
		SDL_Delay(100);
	}

	for (int i = 0; i < pistasAudio; i++) {
		Mix_FreeChunk(channels[i].chunk);
	}

	Mix_CloseAudio();
	Mix_Quit();

	return 0;
}

void generate_object(string seed, float height,int &xcoord,int &ycoord) {
	// Use the random number and the seed to generate a hash.
	size_t hash_value = hash<string>{}(to_string((int)height) + seed);
	// Convert the hash value to a string.
	string hash_string = to_string(hash_value);
	// Split the hash string into two parts.
	string left_part = hash_string.substr(0, 4);
	string right_part = hash_string.substr(4);
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

bool colision(Jugador* j, Powers* pow) {
	bool res = false;
	if (pow->getExist()) {
		if ((j->getPos()->getY() <= pow->getPos()->getY() + pow->getProfCol()) && (j->getPos()->getY() >= pow->getPos()->getY())) {
			if ((j->getPos()->getZ() - j->getAnchCol() <= pow->getPos()->getZ() + pow->getAnchCol()) && (j->getPos()->getZ() + j->getAnchCol() >= pow->getPos()->getZ() - pow->getAnchCol())) {
				if ((j->getPos()->getX() - j->getAltCol() <= pow->getPos()->getX() + pow->getAltCol()) && (j->getPos()->getX() + j->getAltCol() >= pow->getPos()->getX() - pow->getAltCol())) {
					res = true;
				}
			}
		}
	}
	return res;
}


int main(int argc, char* argv[]) {
	//INICIALIZACION
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("ICG-UdelaR Doodle Jump",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	glMatrixMode(GL_PROJECTION);
	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(90, 1280 / 720.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	int vertAmountJugador = 0;
	int vertAmountPlataforma = 0;
	int vertAmountEnemigo = 0;
	int vertAmountBala = 0;
	int vertAmountJetpack = 0;
	int vertAmountShield = 0;
	int vertAmountBackground = 0;
	int dListBackground = -1;
	Vector3** jugador = DoTheImportThing("models/jugador.obj", vertAmountJugador);//mesh.h
	Vector3** plataforma = DoTheImportThing("models/plataforma.obj", vertAmountPlataforma);
	Vector3** enemigo1 = DoTheImportThing("models/enemigo.obj", vertAmountEnemigo);
	Vector3** bala = DoTheImportThing("models/bala.obj", vertAmountBala);
	Vector3** jetpack = DoTheImportThing("models/jetpack.obj", vertAmountJetpack);
	Vector3** shield = DoTheImportThing("models/bala.obj", vertAmountShield);
	Vector3** background = DoTheImportThing("models/background.obj", vertAmountBackground);

	//TEXTURA
	char* archivo = new char[20];
	archivo = "textures/jugador.png";

	//CARGAR IMAGEN
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
	FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wj = FreeImage_GetWidth(bitmap);
	int hj = FreeImage_GetHeight(bitmap);
	void* datosJugador = FreeImage_GetBits(bitmap);

	archivo = "textures/plataforma.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wp = FreeImage_GetWidth(bitmap);
	int hp = FreeImage_GetHeight(bitmap);
	void* datosPlataforma = FreeImage_GetBits(bitmap);

	archivo = "textures/atlasFont.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wa = FreeImage_GetWidth(bitmap);
	int ha = FreeImage_GetHeight(bitmap);
	void* datosAtlasFont = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/Enemigo.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int we = FreeImage_GetWidth(bitmap);
	int he = FreeImage_GetHeight(bitmap);
	void* datosEnemigo = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/Bala.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wb = FreeImage_GetWidth(bitmap);
	int hb = FreeImage_GetHeight(bitmap);
	void* datosBala = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/jetpack.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wje = FreeImage_GetWidth(bitmap);
	int hje = FreeImage_GetHeight(bitmap);
	void* datosJetpack = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/background.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wba = FreeImage_GetWidth(bitmap);
	int hba = FreeImage_GetHeight(bitmap);
	void* datosBackground = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/Bala.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int ws = FreeImage_GetWidth(bitmap);
	int hs = FreeImage_GetHeight(bitmap);
	void* datosShield= FreeImage_GetBits(bitmap);
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
	
	//SONIDOS
	//INICIALIZACION
	SDL_Thread* Audio_thread;
	AudioData channels[pistasAudio];
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cerr << "No se pudo iniciar el mixer de audio: " << Mix_GetError() << endl;
		exit(1);
	}
	for (int i = 0; i < pistasAudio; i++)
	{
		channels[i].boolean_play = new bool;
	}

	//DATOS
	//Efectos de sonido
	channels[0].file = "sounds/jetpack.wav";
	*channels[0].boolean_play = false;

	channels[1].file = "sounds/space-jazz-by-kevin-macleod-from-filmmusic-io.ogg";
	*channels[1].boolean_play = true;

	//CARGA
	for (int i = 0; i < pistasAudio; i++)
	{
		channels[i].channel = new int;
		*channels[i].channel = -1;
		channels[i].chunk = Mix_LoadWAV(channels[i].file);
		if (channels[i].chunk == NULL) {
			cerr << "No se pudo iniciar el audio: " << Mix_GetError() << endl;
			exit(1);
		}
	}

	//REPRODUCCION
	Audio_thread = SDL_CreateThread(AudioThread, "AudioThread", channels);
	if (!Audio_thread) {
		cerr << "No se pudo iniciar Thread de audio: " << SDL_GetError() << endl;
		exit(1);
	}
	//FIN CARGA SONIDOS

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
	float auxX, auxY, auxZ;
	int yAnt = 0;
	float radiansFromMovement = 0;
	float degreesFromMovement = 0;
	float lastDirX = 1, lastDirZ = 0;
	float gradosARotar = 0;
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
	float camRotH = pi/2;
	float camSens = 0.1;//Sensibilidad de la camara
	float radioCamara = 4;//Radio de la camara, se ajusta en juego con la ruedita
	float viewDistance = 5;//Radio alrededor del personaje para el cual se renderizan las plataformas
	float bulletSpeed = 12;
	float jetpackTime = 5;//Duracion del jetpack
	float shieldTime = 15;
	float shakeMagnitude = 0.1;

	Timer* timer = new Timer();//timer para el timeStep
	Timer* jetpackTimer = new Timer();//controla en tiempo que el jugador tiene el jetpack puesto
	Timer* jetpackRemovalTimer = new Timer();//controla el timepo luego de que se le termina el jetpack al jugador
	Timer* shieldTimer = new Timer();
	float jetpackElapsedTime = -1;
	float jetpackRemovedElapsedTime = -1;
	float jetpackTimeToGetToFullGravity = 5;
	float shieldElapsedTime = -1;
	
	float alturaDerrota = -200;

	Vector3 dummy;//Auxiliar para usar operaciones de Vector3

	Vector3* dir = new Vector3(0, 0, 0);//Direccion a la que se tiene que mover el personaje(WASD o FLECHAS)
	Vector3* direccionBala = new Vector3(0, 0, 0);

	float timeAcc = 0;
	float gravity = 0;
	float normalGravity = 10;
	float velocidadJuego = 1;
	float tiempoTranscurrido = 0;
	float score = 0;
	float altAlcanzada = 0;
	float probEnemigos = 19;
	float probJetpack = 5;
	float probEscudo = 15;
	string seed = "sdda";

	//Generacion de plataformas
	Plataforma* choque = NULL;
	Plataforma** plataformas = new Plataforma*[11];
	int cantPlat = 11;
	//Generacion de enemigos
	Enemigo* colEnemigo = NULL;
	Enemigo* enemigoHerido = NULL;
	Enemigo** enemigos = new Enemigo*[11];
	enemigos[0] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[0]->setPos(-11, -11, -11);
	enemigos[1] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[1]->setPos(-11, -11, -11);
	enemigos[2] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[2]->setPos(-11, -11, -11);
	enemigos[3] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[3]->setPos(-11, -11, -11);
	enemigos[4] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[4]->setPos(-11, -11, -11);
	enemigos[5] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[5]->setPos(-11, -11, -11);
	enemigos[6] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[6]->setPos(-11, -11, -11);
	enemigos[7] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[7]->setPos(-11, -11, -11);
	enemigos[8] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[8]->setPos(-11, -11, -11);
	enemigos[9] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[9]->setPos(-11, -11, -11);
	enemigos[10] = new Enemigo(0.3, 0.3, 0.3);
	enemigos[10]->setPos(-11, -11, -11);
	int cantEnem = 11;

	//Generacion de jetpack
	bool colJetpack = false;
	Jetpack* jetp = new Jetpack(1, 1, 1);
	jetp->setPos(-11, -11, -11);

	//Generacion de escudo
	bool colEscudo = false;
	bool damageOn = true;
	Shield* escudo = new Shield(1, 1, 1);
	escudo->setPos(-11, -11, -11);

	Powers** poderes = new Powers*[2];
	poderes[0] = jetp;
	poderes[1] = escudo;
	Vector3*** modelosPoderes = new Vector3**[2];
	modelosPoderes[0] = jetpack;
	modelosPoderes[1] = shield;
	int* vertPoderes = new int[2];
	vertPoderes[0] = vertAmountJetpack;
	vertPoderes[1] = vertAmountShield;

	
	
	
	




	//Se crea el jugador
	Jugador* jug = new Jugador(0.3, 0.3, 0.2);
	//Se crea la bala
	Bullet* bul = new Bullet(0, 0, 0, 0.1, 0.1, 0.5);

	
	//LOOP PRINCIPAL

	for (int i = 0; i < 11; i++) {
		int xcoord = 2, zcoord = 0;
		generate_object(seed, i, xcoord, zcoord);
		srand(xcoord + zcoord + i + 5);
		int prob = rand() % 100;
		xcoord = (xcoord % 100) * 0.06;
		zcoord = (zcoord % 100) * 0.06;
		if (100 - prob < probEnemigos) {
			enemigos[i % 11]->setPos(xcoord, i + 0.4, zcoord);
			if (i > jug->getPos()->getY() + 1)enemigos[i % 11]->setExists(true);
		}
		plataformas[i % 11] = new Plataforma(xcoord, i, zcoord, 1.4, 0.5, 0.3, 'n');
	}
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		x = radioCamara * cos(camRot);
		y = radioCamara * cos(camRotH);
		z = radioCamara * sin(camRot);
		if (camType) //Se elige el tipo de camara(con V)
			if (jetp->getOnPlayer())
				gluLookAt(x + jug->getPos()->getX(), y + 1.5 + jug->getPos()->getY(), z + jug->getPos()->getZ(), jug->getPos()->getX() + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, jug->getPos()->getY() + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, jug->getPos()->getZ() + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, 0, 1, 0);//Camara centrada en el jugador con sacudida
			else
				gluLookAt(x + jug->getPos()->getX(), y + 1.5 + jug->getPos()->getY(), z + jug->getPos()->getZ(), jug->getPos()->getX(), jug->getPos()->getY(), jug->getPos()->getZ(), 0, 1, 0);//Camara centrada en el jugador
		else if(jetp->getOnPlayer())
			gluLookAt(jug->getPos()->getX(),jug->getPos()->getY() + 1, jug->getPos()->getZ(), jug->getPos()->getX() - x + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, jug->getPos()->getY() + y + 1 + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, jug->getPos()->getZ() - z + 1 + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, 0, 1, 0);//Camara centrada en el escenario con sacudida
		else
			gluLookAt(jug->getPos()->getX(), jug->getPos()->getY() + 1, jug->getPos()->getZ(), jug->getPos()->getX() - x, jug->getPos()->getY() + y + 1, jug->getPos()->getZ() - z + 1, 0, 1, 0);//Camara centrada en el escenario
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
		*channels[0].boolean_play = jetp->getOnPlayer();

		if(jug->getPos()->getY() > 5)
			for (int i = jug->getPos()->getY() - 5; i < jug->getPos()->getY() + 5; i++) {
				int xcoord = 0, zcoord = 0;
				generate_object(seed, i, xcoord, zcoord);
				srand(xcoord + zcoord +i +5);
				int prob = rand() % 100;
				xcoord = (xcoord % 100) * 0.06;
				zcoord = (zcoord % 100) * 0.06;
				if ((100 - prob < probEnemigos) && (enemigos[i % 11]->getPos()->getY() < i )) {
					enemigos[i % 11]->setPos(xcoord,i+0.4,zcoord);
				}
				if ((100 - prob < probJetpack) && (jetp->getPos()->getY() == -11) && i > jug->getPos()->getY()) {
					jetp->setPos(xcoord, i + 0.4, zcoord);
				}
				if ((100 - prob < probEscudo) && (escudo->getPos()->getY() == -11) && i > jug->getPos()->getY()) {
					escudo->setPos(xcoord, i + 0.4, zcoord);
				}
				plataformas[i % 11]->setPos(xcoord, i, zcoord);
			}
		//TRANSFORMACIONES LINEALES
		jug->setVel(dummy.multVecEsc(*dummy.normalize(*dir), moveSpeed));//Se normaliza la dirección de movimiento y se le asigna la velocidad
		if (jug->getPos()->getY() >= 0 ) { //Se reduce la velocidad en función de la gravedad si el personaje se encuentra saltando
			timeAcc += timeStep;
			jug->getVel()->setY(jumpSpeed - timeAcc * gravity);
			choque = colision(jug, plataformas, cantPlat);//Chequeo de colision con plataformas
			if (damageOn)
				colEnemigo = colision(jug, enemigos, cantEnem);//Chequeo de colision con enemigos
			enemigoHerido = colision(bul, enemigos, cantEnem);//Chequeo de colision de bala con enemigo
			colJetpack = colision(jug, jetp);//
			colEscudo = colision(jug, escudo);
			if (colJetpack) {
				//Si colisiono con el jetpack me lo pongo y arranco el timer
				jetp->setOnPlayer(true);
				if (jetpackElapsedTime == -1) {
					jetpackTimer->peek();
					jetpackElapsedTime = 0;
				}
			}
			if (colEscudo) {
				escudo->setOnPlayer(true);
				damageOn = false;
				if (shieldElapsedTime == -1) {
					shieldTimer->peek();
					shieldElapsedTime = 0;
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
			jug->setPos(5,0,5);
			yAnt = 0;
			altAlcanzada = 0;
			//Se marcan todas las plataformas como existentes nuevamente
			for (int i = 0; i < 11; i++) {
				int xcoord = 2, zcoord = 0;
				generate_object(seed, i, xcoord, zcoord);
				srand(xcoord + zcoord + i + 5);
				int prob = rand() % 100;
				xcoord = (xcoord % 100) * 0.06;
				zcoord = (zcoord % 100) * 0.06;
				if (100 - prob < probEnemigos) {
					enemigos[i % 11]->setPos(xcoord, i + 0.4, zcoord);
				}
				plataformas[i % 11]->setPos(xcoord,i,zcoord);
			}
			for (int i = 0; i < cantPlat; i++) {
				plataformas[i]->setExists(true);
			}
			for (int i = 0; i < cantEnem; i++) {
				enemigos[i]->setExists(true);
			}
			bul->getPos()->setY(0);
			jetp->setExist(true);
			jetp->setPos(-11, -11, -11);
			jetp->setOnPlayer(false);
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
		radiansFromMovement = atan2(x, z);
		if(movingb || movingf || movingr || movingl)
			degreesFromMovement = radiansFromMovement * 180 / M_PI;
		if (movingb)
			degreesFromMovement = degreesFromMovement - 180;
		if (movingr)
			degreesFromMovement = degreesFromMovement - 90;
		if (movingl)
			degreesFromMovement = degreesFromMovement + 90;
		if (degreesFromMovement < 0) degreesFromMovement += 360;
		glRotatef(degreesFromMovement + 90, 0, 1, 0);
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
				jetpackRemovalTimer->peek();
				jetpackRemovedElapsedTime = 0;
				jetp->setPos(-11, -11, -11);
			}
		}
		else {
			if (jetpackRemovedElapsedTime != -1) {
				//segundos luego de que se acabe el jetpack tengo que aumentar la gravedad gradualmente.
				jetpackRemovedElapsedTime += jetpackRemovalTimer->touch().delta;
				if (jetpackRemovedElapsedTime <= jetpackTimeToGetToFullGravity) {
					float gravityMultiplier = jetpackRemovedElapsedTime / jetpackTimeToGetToFullGravity;
					gravity = normalGravity * gravityMultiplier;
				}
				else
					jetpackRemovedElapsedTime = -1;
			}
			else {
				gravity = normalGravity;
			}
			if (jetp->getPos()->getY() < jug->getPos()->getY() - 10 && jetp->getPos()->getY() != -11) jetp->setPos(-11,-11,-11);
		}

		//Checkeo timer del escudo
		if (escudo->getOnPlayer()) {
			shieldElapsedTime += shieldTimer->touch().delta;
			if (shieldElapsedTime >= shieldTime) {
				//Se acabo el tiempo del escudo
				shieldElapsedTime = -1;
				escudo->setOnPlayer(false);
				damageOn = true;
				escudo->setPos(1, 1, 1);
			}
		}

		//Dibujado de poderes
		for (int i = 0; i < 2; i++) {
			if (poderes[i]->getExist()) {
				glPushMatrix();
				glTranslatef(poderes[i]->getPos()->getX(), poderes[i]->getPos()->getY(), poderes[i]->getPos()->getZ());
				if (poderes[i]->getOnPlayer()) {
					poderes[i]->setPos(jug->getPos()->getX(), jug->getPos()->getY(), jug->getPos()->getZ());
					if (dir->getModulo() != 0) {
						if (dir->getZ() > 0) gradosARotar = acos(dummy.dot(*dir, Vector3(1, 0, 0)) / (dir->getModulo() * Vector3(1, 0, 0).getModulo())) * 57.2958;
						else gradosARotar = 360 - acos(dummy.dot(*dir, Vector3(1, 0, 0)) / (dir->getModulo() * Vector3(1, 0, 0).getModulo())) * 57.2958;
					}
					glRotatef(-gradosARotar + 180, 0, 1, 0);
				}
				glTranslatef(0.4, 0.2, 0.0);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wje, hje, 0, GL_BGR, GL_UNSIGNED_BYTE, datosJetpack);
				if ((camType || !poderes[i]->getOnPlayer()) && (poderes[i]->getPos()->getY() > jug->getPos()->getY() - viewDistance) && (poderes[i]->getPos()->getY() < jug->getPos()->getY() + viewDistance))
					poderes[i]->draw(modelosPoderes[i], vertPoderes[i], textura);
				glPopMatrix();
			}
		}
		
	
		//DIBUJO ESCENARIO(Sin movimiento de personaje)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
		glPushMatrix();
		for (int i = 0; i < cantPlat; i++) {
			if ((plataformas[i]->getY() > jug->getPos()->getY() - viewDistance) && (plataformas[i]->getY() < jug->getPos()->getY() + viewDistance) && plataformas[i]->getExists()) {
				switch (plataformas[i]->getType()) {
				case 'n':
					plataformas[i]->draw(plataforma, vertAmountPlataforma, textura);
					break;
				case 'd':
					glColor3f(1, 0, 0.412);
					plataformas[i]->draw(plataforma, vertAmountPlataforma, textura);
					glColor3f(1, 1, 1);
					break;
				}
			}
		}
		glPopMatrix();
		glPushMatrix();
		glTranslatef(3.0, jug->getPos()->getY(), 3.0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wba, hba, 0, GL_BGR, GL_UNSIGNED_BYTE, datosBackground);
		if (dListBackground == -1) dListBackground = drawFaces(background,vertAmountBackground,textura);
		if(dListBackground != -1) glCallList(dListBackground);
		glPopMatrix();

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
			if (enemigos[i]->getExists() && enemigos[i]->getPos()->getY() <= jug->getPos()->getY() + viewDistance && enemigos[i]->getPos()->getY() >= jug->getPos()->getY() - viewDistance) {
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
		if (escudo->getOnPlayer())
			renderShieldTime(shieldTime - shieldElapsedTime, textura);
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
				camRot = fmod((camRot + evento.motion.xrel * camSens * timeStep), pi * 2);
				if(camRotH < pi-0.1 && camRotH > 0)camRotH = fmod((camRotH + evento.motion.yrel * camSens*0.3*timeStep), pi);
				if (camRotH <= 0) camRotH = camRotH + 0.01;
				if (camRotH >= pi-0.1) camRotH = camRotH - 0.01;
				break;
			case SDL_QUIT:
				fin = true;
				break;
			case SDL_KEYDOWN:
				switch (evento.key.keysym.sym) {
				case SDLK_RIGHT:
				case SDLK_d:
					auxX = pow(cos(camRot), 2);
					auxZ = pow(sin(camRot), 2);
					lastDirX = sin(camRot) / sqrt(auxX + auxZ);
					lastDirZ = -cos(camRot) / sqrt(auxX + auxZ);
					dir->setZ(lastDirZ);
					dir->setX(lastDirX);
					movingr = true;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					auxX = pow(cos(camRot), 2);
					auxZ = pow(sin(camRot), 2);
					lastDirX = -sin(camRot) / sqrt(auxX + auxZ);
					lastDirZ = cos(camRot) / sqrt(auxX + auxZ);
					dir->setZ(lastDirZ);
					dir->setX(lastDirX);
					movingl = true;
					break;
				case SDLK_UP:
				case SDLK_w:
					auxX = pow(cos(camRot),2);
					auxZ = pow(sin(camRot),2);
					lastDirX = -cos(camRot) / sqrt(auxX + auxZ);
					lastDirZ = -sin(camRot) / sqrt(auxX + auxZ);
					dir->setZ(lastDirZ);
					dir->setX(lastDirX);
					movingf = true;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					auxX = pow(cos(camRot), 2);
					auxZ = pow(sin(camRot), 2);
					lastDirX = cos(camRot) / sqrt(auxX + auxZ);
					lastDirZ = sin(camRot) / sqrt(auxX + auxZ);
					dir->setZ(lastDirZ);
					dir->setX(lastDirX);
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
						direccionBala->setX(lastDirX);
						direccionBala->setY(dir->getY());
						direccionBala->setZ(lastDirZ);
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
					if (!movingb && !movingf && !movingl) { dir->setZ(0); dir->setX(0); }
					movingr = false;
					break;
				case SDLK_LEFT:
				case SDLK_a:
					if (!movingb && !movingr && !movingf) { dir->setZ(0); dir->setX(0); }
					movingl = false;
					break;
				case SDLK_UP:
				case SDLK_w:
					if (!movingb && !movingr && !movingl) { dir->setZ(0); dir->setX(0);}
					movingf = false;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					if (!movingf && !movingr && !movingl) { dir->setZ(0); dir->setX(0); }
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
	playing = false;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	int* exitAudio = new int;
	SDL_WaitThread(Audio_thread, exitAudio);
	SDL_Quit();
	delete timer;
	return 0;
}
