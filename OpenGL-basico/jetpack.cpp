#include "jetpack.h"

Jetpack::Jetpack(float alt, float anch, float prof) {
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	pos = new Vector3(0, 0, 0);
	disList = -1;
	exist = true;
	onPlayer = false;
}

float Jetpack::getAltCol() {
	return altCol;
}

float Jetpack::getAnchCol() {
	return anchCol;
}

float Jetpack::getProfCol() {
	return profCol;
}

Vector3* Jetpack::getPos() {
	return pos;
}

bool Jetpack::getExist() {
	return exist;
}

bool Jetpack::getOnPlayer() {
	return onPlayer;
}

void Jetpack::setPos(Vector3* vect) {
	pos = vect;
}

void Jetpack::setPos(float x, float y, float z) {
	pos->setX(x);
	pos->setY(y);
	pos->setZ(z);
}

void Jetpack::setExist(bool val) {
	exist = val;
}

void Jetpack::setOnPlayer(bool val) {
	onPlayer = val;
}


void Jetpack::draw(Vector3** modelo, int caras, GLuint textura) {
	if (disList == -1) disList = drawFaces(modelo, caras, textura);
	if (disList != -1) glCallList(disList);
}
