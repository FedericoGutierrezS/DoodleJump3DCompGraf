#include "powers.h"

Powers::Powers(float alt, float anch, float prof) {
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	pos = new Vector3(0, 0, 0);
	disList = -1;
	exist = true;
	onPlayer = false;
}

Powers::~Powers() {
	delete pos;
}

float Powers::getAltCol() {
	return altCol;
}

float Powers::getAnchCol() {
	return anchCol;
}

float Powers::getProfCol() {
	return profCol;
}

Vector3* Powers::getPos() {
	return pos;
}

bool Powers::getExist() {
	return exist;
}

bool Powers::getOnPlayer() {
	return onPlayer;
}

void Powers::setPos(Vector3* vect) {
	pos = vect;
}

void Powers::setPos(float x, float y, float z) {
	pos->setX(x);
	pos->setY(y);
	pos->setZ(z);
}

void Powers::setExist(bool val) {
	exist = val;
}

void Powers::setOnPlayer(bool val) {
	onPlayer = val;
}


void Powers::draw(Vector3** modelo, int caras, GLuint textura) {
	if (disList == -1) disList = drawFaces(modelo, caras, textura);
	if (disList != -1) glCallList(disList);
}