#include "jugador.h"


Jugador::Jugador(float alt, float anch, float prof) {
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	pos = new Vector3(0, 0, 0);
	vel = new Vector3(0, 0, 0);
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

void Jugador::draw(Vector3** modelo, int caras, GLuint textura) {
	glPushMatrix();
	glTranslatef(pos->getX(), pos->getY(), pos->getZ());
	drawFaces(modelo, caras, textura);
	glPopMatrix();
}