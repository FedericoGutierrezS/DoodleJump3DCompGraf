#include "plataforma.h"


Plataforma::Plataforma(float a, float b, float c, float alt, float anch, float prof) {
	x = a;
	y = b;
	z = c;
	altCol = alt;
	anchCol = anch;
	profCol = prof;
}
float Plataforma::getX() {
	return x;
}
float Plataforma::getY() {
	return y;
}
float Plataforma::getZ() {
	return z;
}
float Plataforma::getAltCol() {
	return altCol;
}
float Plataforma::getAnchCol() {
	return anchCol;
}
float Plataforma::getProfCol() {
	return profCol;
}
void Plataforma::draw(Vector3** modelo, int caras,GLuint textura) {
	glPushMatrix();
	glTranslatef(x, y, z);
	drawFaces(modelo, caras, textura);
	glPopMatrix();
}