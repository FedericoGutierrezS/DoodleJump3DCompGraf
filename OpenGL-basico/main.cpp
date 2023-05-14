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
#include "Particle.h"
#include "ParticleEmitter.h"
#include <algorithm>
#include <stdlib.h>
#include <string>

using namespace std;

//Cantidad maxima de pistas de audio, debe ser mayor o igual a la cantidad de audios que tiene el juego
const int pistasAudio = 2;

//Indica si termino el juego o no
bool fin = false;

struct AudioData {
	Mix_Chunk* chunk;
	bool* boolean_play;
	const char* file;
	int* channel;
};

int AudioThread(void* data) {
	AudioData* channels = static_cast<AudioData*>(data);
	while (!fin) {
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

	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	for (int y = 0; y < hp; y++) {
		BYTE* pixel = FreeImage_GetScanLine(bitmap, y);
		for (int x = 0; x < wp; x++) {
			pixel[0] /= 1.7;
			pixel[1] /= 1.7;
			pixel[2] = 255;

			pixel += 3;
		}
	}
	void* datosPlataforma2 = FreeImage_GetBits(bitmap);

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

	archivo = "textures/buttoncolb.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbucb = FreeImage_GetWidth(bitmap);
	int hbucb = FreeImage_GetHeight(bitmap);
	void* datosButColB = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttoncolg.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbucg = FreeImage_GetWidth(bitmap);
	int hbucg = FreeImage_GetHeight(bitmap);
	void* datosButColG = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttoncolr.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbucr = FreeImage_GetWidth(bitmap);
	int hbucr = FreeImage_GetHeight(bitmap);
	void* datosButColR = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttoncolv.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbucv = FreeImage_GetWidth(bitmap);
	int hbucv = FreeImage_GetHeight(bitmap);
	void* datosButColV = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttoncoly.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbucy = FreeImage_GetWidth(bitmap);
	int hbucy = FreeImage_GetHeight(bitmap);
	void* datosButColY = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttonvel.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbuvel = FreeImage_GetWidth(bitmap);
	int hbuvel = FreeImage_GetHeight(bitmap);
	void* datosButVel = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttontex.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbutex = FreeImage_GetWidth(bitmap);
	int hbutex = FreeImage_GetHeight(bitmap);
	void* datosButTex = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttonwire.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbuwire = FreeImage_GetWidth(bitmap);
	int hbuwire = FreeImage_GetHeight(bitmap);
	void* datosButWire = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttonint.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbuint = FreeImage_GetWidth(bitmap);
	int hbuint = FreeImage_GetHeight(bitmap);
	void* datosButInt = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttonfac.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbufac = FreeImage_GetWidth(bitmap);
	int hbufac = FreeImage_GetHeight(bitmap);
	void* datosButFac = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/buttondir.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int wbudir = FreeImage_GetWidth(bitmap);
	int hbudir = FreeImage_GetHeight(bitmap);
	void* datosButDir = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/0.5.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int w05 = FreeImage_GetWidth(bitmap);
	int h05 = FreeImage_GetHeight(bitmap);
	void* datos05 = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/0.25.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int w025 = FreeImage_GetWidth(bitmap);
	int h025 = FreeImage_GetHeight(bitmap);
	void* datos025 = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/1.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int w1 = FreeImage_GetWidth(bitmap);
	int h1 = FreeImage_GetHeight(bitmap);
	void* datos1 = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/2.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int w2 = FreeImage_GetWidth(bitmap);
	int h2 = FreeImage_GetHeight(bitmap);
	void* datos2 = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/on.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int won = FreeImage_GetWidth(bitmap);
	int hon = FreeImage_GetHeight(bitmap);
	void* datosOn = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	archivo = "textures/off.png";

	//CARGAR IMAGEN
	fif = FreeImage_GetFIFFromFilename(archivo);
	bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int woff = FreeImage_GetWidth(bitmap);
	int hoff = FreeImage_GetHeight(bitmap);
	void* datosOff = FreeImage_GetBits(bitmap);
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
	int xPos = 0;
	int yPos = 0;


	Vector3* colores = new Vector3[5];
	colores[0] = Vector3(1, 1, 1);
	colores[1] = Vector3(0.8, 0, 0.8);
	colores[2] = Vector3(0.8, 0, 0);
	colores[3] = Vector3(0, 0, 0.8);
	colores[4] = Vector3(0, 0.8, 0);
	int luzAct = 0;
	int luzActPos = 0;

	GLfloat luz_posicion[4] = { 0, 3, 1, 1 };
	GLfloat luz_posicion1[4] = { 0, 3, 1, 1 };
	GLfloat colorLuz1[4] = { 1, 1, 1, 0 };
	GLfloat colorLuz2[4] = { colores[0].getX(), colores[0].getY(), colores[0].getZ(), 1 };

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
	float* velocidades = new float[4];
	velocidades[0] = 0.25;
	velocidades[1] = 0.5;
	velocidades[2] = 1;
	velocidades[3] = 2;
	float* direcciones = new float[4];
	direcciones[0] = 4;
	direcciones[1] = -4;
	direcciones[2] = 4;
	direcciones[3] = -4;


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
	int velocidadJuegoPos = 2;
	float tiempoTranscurrido = 0;
	float score = 0;
	float altAlcanzada = 0;
	float probPlataformaRota = 10;
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

	//Generacion de particulas
	//Test particula
	//Particle* testParticula = new Particle(new Vector3(1, 5, 5), new Vector3(1, 0, 0), 0.5, 1, 9.8, 20, true);
	ParticleEmitter* testEmitter = new ParticleEmitter(new Vector3(1, 1, 5), new Vector3(0, 0.1, 0), new Vector3(1, 0, 0), 0.5, new Vector3(1, 0, 0), 0.5, 30, 0.1, 1, 3, -2, true, false);

	for (int i = 0; i < 11; i++) {
		int xcoord = 2, zcoord = 0;
		generate_object(seed, i, xcoord, zcoord);
		srand((xcoord + zcoord + i + 5) * 10);
		int prob = rand() % 100;
		int probPlat = rand() % 100;
		xcoord = (xcoord % 100) * 0.06;
		zcoord = (zcoord % 100) * 0.06;
		if (100 - prob < probEnemigos) {
			enemigos[i % 11]->setPos(xcoord, i + 0.4, zcoord);
			if (i > jug->getPos()->getY() + 1)enemigos[i % 11]->setExists(true);
		}
		if (100 - probPlat < probPlataformaRota)
			plataformas[i % 11] = new Plataforma(xcoord, i, zcoord, 1.4, 0.5, 0.3, 'd');
		else
			plataformas[i % 11] = new Plataforma(xcoord, i, zcoord, 1.4, 0.5, 0.3, 'n');
	}

	//LOOP PRINCIPAL
	float fps = 0;
	int frameCount = 0;
	Uint64 startTime = SDL_GetPerformanceCounter();
	Uint64 prevTime = startTime;

	do {
		if (!pause) SDL_SetRelativeMouseMode(SDL_TRUE);
		else SDL_SetRelativeMouseMode(SDL_FALSE);
		//Actualizo contador de fps al inicio de cada frame
		Uint64 currentTime = SDL_GetPerformanceCounter();
		float deltaTime = static_cast<float>(currentTime - prevTime) / SDL_GetPerformanceFrequency();
		if (frameCount % 10 == 0)
			fps = 1.0 / deltaTime;
		prevTime = currentTime;
		frameCount++;

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
		else if (jetp->getOnPlayer())
			gluLookAt(jug->getPos()->getX(), jug->getPos()->getY() + 1, jug->getPos()->getZ(), jug->getPos()->getX() - x + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, jug->getPos()->getY() + y + 1 + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, jug->getPos()->getZ() - z + 1 + ((((float)rand() / (float)RAND_MAX) * 2.0) - 1.0) * shakeMagnitude, 0, 1, 0);//Camara centrada en el escenario con sacudida
		else
			gluLookAt(jug->getPos()->getX(), jug->getPos()->getY() + 1, jug->getPos()->getZ(), jug->getPos()->getX() - x, jug->getPos()->getY() + y + 1, jug->getPos()->getZ() - z + 1, 0, 1, 0);//Camara centrada en el escenario
		//PRENDO LA LUZ (SIEMPRE DESPUES DEL gluLookAt)

		colorLuz2[0] = colores[luzAct].getX();
		colorLuz2[1] = colores[luzAct].getY();
		colorLuz2[2] = colores[luzAct].getZ();

		glEnable(GL_LIGHT0); // habilita la luz 0
		glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLuz1);
		glEnable(GL_LIGHT1); // habilita la luz 1
		glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, colorLuz2);

		timeStep = velocidades[velocidadJuegoPos] * timer->touch().delta;
		if (pause) timeStep = 0;//Pausa
		tiempoTranscurrido = tiempoTranscurrido + timeStep;//Contador de tiempo
		*channels[0].boolean_play = jetp->getOnPlayer();

		if (jug->getPos()->getY() > 5)
			for (int i = jug->getPos()->getY() - 5; i < jug->getPos()->getY() + 5; i++) {
				int xcoord = 0, zcoord = 0;
				generate_object(seed, i, xcoord, zcoord);
				srand((xcoord + zcoord + i + 5) * 10);
				int prob = rand() % 100;
				int probPlat = rand() % 100;
				xcoord = (xcoord % 100) * 0.06;
				zcoord = (zcoord % 100) * 0.06;
				if ((100 - prob < probEnemigos) && (enemigos[i % 11]->getPos()->getY() < i) && (100 - probPlat >= probPlataformaRota)) {
					enemigos[i % 11]->setPos(xcoord, i + 0.4, zcoord);
				}
				if ((100 - prob < probJetpack) && (jetp->getPos()->getY() == -11) && i > jug->getPos()->getY()) {
					jetp->setPos(xcoord, i + 0.4, zcoord);
				}
				if ((100 - prob < probEscudo) && (escudo->getPos()->getY() == -11) && i > jug->getPos()->getY()) {
					escudo->setPos(xcoord, i + 0.4, zcoord);
				}
				if (100 - probPlat < probPlataformaRota) {
					delete plataformas[i % 11];
					plataformas[i % 11] = new Plataforma(xcoord, i, zcoord, 1.4, 0.5, 0.3, 'd');
				}
				plataformas[i % 11]->setPos(xcoord, i, zcoord);
			}
		//TRANSFORMACIONES LINEALES
		jug->setVel(dummy.multVecEsc(*dummy.normalize(*dir), moveSpeed));//Se normaliza la dirección de movimiento y se le asigna la velocidad
		if (jug->getPos()->getY() >= 0) { //Se reduce la velocidad en función de la gravedad si el personaje se encuentra saltando
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
				if (alturaDerrota < choque->getY() - 2) alturaDerrota = choque->getY() - 2;
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
			delete testEmitter;
			testEmitter = new ParticleEmitter(new Vector3(enemigoHerido->getPos()->getX(), enemigoHerido->getPos()->getY() + 0.2, enemigoHerido->getPos()->getZ()), new Vector3(0, 0.1, 0), new Vector3(0.2, 0, 1), 0.5, new Vector3(1, 0, 0), 0.5, 30, 0.1, 1, 3, -2, true, false);
			enemigoHerido = NULL;
		}

		//Condiciones de Derrota
		if (jug->getPos()->getY() < alturaDerrota || colEnemigo != NULL) {
			alturaDerrota = -200;
			tiempoTranscurrido = 0;
			score = 0;
			jug->setPos(5, 0, 5);
			yAnt = 0;
			altAlcanzada = 0;
			//Se marcan todas las plataformas como existentes nuevamente
			for (int i = 0; i < 11; i++) {
				int xcoord = 2, zcoord = 0;
				generate_object(seed, i, xcoord, zcoord);
				srand((xcoord + zcoord + i + 5) * 10);
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
		luz_posicion[0] = jug->getPos()->getX() + 0;
		luz_posicion[1] = jug->getPos()->getY() + 5;
		luz_posicion[2] = jug->getPos()->getZ() + 0;
		if (luzActPos % 4 < 2) {
			luz_posicion1[0] = jug->getPos()->getX() + direcciones[luzActPos];
			luz_posicion1[2] = jug->getPos()->getZ() + 0;
		}
		if (luzActPos % 4 > 1) {
			luz_posicion1[0] = jug->getPos()->getX() + 0;
			luz_posicion1[2] = jug->getPos()->getZ() + direcciones[luzActPos];
		}
		luz_posicion1[1] = jug->getPos()->getY() + 2;
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
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma2);
					plataformas[i]->draw(plataforma, vertAmountPlataforma, textura);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wp, hp, 0, GL_BGR, GL_UNSIGNED_BYTE, datosPlataforma);
					break;
				}
			}
		}

		//DIBUJADO DE PARTICULAS
		//test particulas
		//testParticula->draw(x + jug->getPos()->getX(), y + jug->getPos()->getY(), z + jug->getPos()->getZ());
		testEmitter->draw(0, 0, 0, tiempoTranscurrido);

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
		glDisable(GL_LIGHTING);

		glPushMatrix();
		glTranslatef(3.0, jug->getPos()->getY(), 3.0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wba, hba, 0, GL_BGR, GL_UNSIGNED_BYTE, datosBackground);
		if (dListBackground == -1) dListBackground = drawFaces(background,vertAmountBackground,textura);
		if(dListBackground != -1) glCallList(dListBackground);
		glPopMatrix();
		
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
		if (pause) {
			glEnable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbuvel, hbuvel, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButVel);
			renderButton(400, 30, textura);
			switch ((velocidadJuegoPos) % 4) {
			case 0:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w025, h025, 0, GL_BGR, GL_UNSIGNED_BYTE, datos025);
				break;
			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w05, h05, 0, GL_BGR, GL_UNSIGNED_BYTE, datos05);
				break;
			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w1, h1, 0, GL_BGR, GL_UNSIGNED_BYTE, datos1);
				break;
			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w2, h2, 0, GL_BGR, GL_UNSIGNED_BYTE, datos2);
				break;
			}
			renderValue(730, 30, textura);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbutex, hbutex, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButTex);
			renderButton(400, 130, textura);
			if (texturas)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, won, hon, 0, GL_BGR, GL_UNSIGNED_BYTE, datosOn);
			else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, woff, hoff, 0, GL_BGR, GL_UNSIGNED_BYTE, datosOff);
			renderValue(730, 130, textura);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbuwire, hbuwire, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButWire);
			renderButton(400, 230, textura);
			if (wireframe)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, won, hon, 0, GL_BGR, GL_UNSIGNED_BYTE, datosOn);
			else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, woff, hoff, 0, GL_BGR, GL_UNSIGNED_BYTE, datosOff);
			renderValue(730, 230, textura);

			if (!facetado) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbuint, hbuint, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButInt);
			else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbufac, hbufac, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButFac);
			renderButton(400, 330, textura);


			switch ((luzAct) % 5) {
			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbucv, hbucv, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButColV);
				break;
			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbucr, hbucr, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButColR);
				break;
			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbucb, hbucb, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButColB);
				break;
			case 4:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbucg, hbucg, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButColG);
				break;
			case 0:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbucy, hbucy, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButColY);
				break;
			}
			renderButton(400, 500, textura);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wbudir, hbudir, 0, GL_BGR, GL_UNSIGNED_BYTE, datosButDir);
			renderButton(400, 600, textura);

			renderBackground();
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wa, ha, 0, GL_BGR, GL_UNSIGNED_BYTE, datosAtlasFont);
			renderTime(tiempoTranscurrido, textura);
			renderScore(score, textura);
			renderFrames(fps, textura);
			if (escudo->getOnPlayer()) renderShieldTime(shieldTime - shieldElapsedTime, textura);
		}
		// Making sure we can render 3d again
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_TEXTURE_2D);
		
		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)) {
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&xPos, &yPos);

				if (xPos > 400 && xPos < 700 && yPos > 30 && yPos < 105 && pause) velocidadJuegoPos = (velocidadJuegoPos + 1) % 4;

				if (xPos > 400 && xPos < 700 && yPos > 130 && yPos < 205 && pause) texturas = !texturas;

				if (xPos > 400 && xPos < 700 && yPos > 230 && yPos < 305 && pause) wireframe = !wireframe;

				if (xPos > 400 && xPos < 700 && yPos > 330 && yPos < 405 && pause) facetado = !facetado;

				if (xPos > 400 && xPos < 700 && yPos > 500 && yPos < 575 && pause) luzAct = (luzAct + 1) % 5;

				if (xPos > 400 && xPos < 700 && yPos > 600 && yPos < 675 && pause) luzActPos = (luzActPos + 1) % 4;;

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
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	int* exitAudio = new int;
	SDL_WaitThread(Audio_thread, exitAudio);
	SDL_Quit();
	for (int i = 0; i < 11; i++) {
		delete plataformas[i % 11];
	}
	delete exitAudio;
	delete timer;
	delete jug;
	delete bul;
	delete[] enemigos;
	delete[] colores;
	delete[] velocidades;
	delete[] direcciones;
	delete jetpackTimer;
	delete jetpackRemovalTimer;
	delete shieldTimer;
	delete dir;
	delete direccionBala;
	delete jetp;
	delete escudo;
	delete[] poderes;
	delete[] vertPoderes;
	delete[] modelosPoderes;
	for (int i = 0; i < pistasAudio; i++)
	{
		delete channels[i].boolean_play;
		delete channels[i].channel;
	}
	for (int i = 0; i < 3; ++i) {
		delete[] jugador[i];
		delete[] plataforma[i];
		delete[] enemigo1[i];
		delete[] bala[i];
		delete[] jetpack[i];
		delete[] shield[i];
		delete[] background[i];
	}
	delete[] jugador;
	delete[] plataforma;
	delete[] enemigo1;
	delete[] bala;
	delete[] jetpack;
	delete[] shield;
	delete[] background;
	return 0;
}
