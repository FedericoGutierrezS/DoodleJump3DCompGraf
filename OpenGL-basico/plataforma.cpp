#include "plataforma.h"


Plataforma::Plataforma(float a, float b, float c, float alt, float anch, float prof) {
	x = a;
	y = b;
	z = c;
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	disList = -1;
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
	if(disList == -1) disList = drawFaces(modelo, caras, textura);
	if (disList != -1) glCallList(disList);
	glPopMatrix();
}