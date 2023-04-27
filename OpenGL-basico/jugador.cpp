#include "jugador.h"


Jugador::Jugador(float alt, float anch, float prof) {
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	pos = new Vector3(0, 0, 0);
	vel = new Vector3(0, 0, 0);
	disList = -1;
}

float Jugador::getAltCol() {
	return altCol;
}
float Jugador::getAnchCol() {
	return anchCol;
}
float Jugador::getProfCol() {
	return profCol;
}

Vector3* Jugador::getVel() {
	return vel;
}

void Jugador::setVel(Vector3* vect) {
	vel = vect;
}

Vector3* Jugador::getPos() {
	return pos;
}

void Jugador::setPos(Vector3* vect) {
	pos = vect;
}

void Jugador::setPos(float x,float y,float z) {
	pos->setX(x);
	pos->setY(y);
	pos->setZ(z);
}

void Jugador::draw(Vector3** modelo, int caras, GLuint textura) {
	if(disList == -1) disList = drawFaces(modelo, caras, textura);
	if (disList != -1) glCallList(disList);
}