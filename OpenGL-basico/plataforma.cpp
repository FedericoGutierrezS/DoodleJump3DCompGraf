#include "plataforma.h"


Plataforma::Plataforma(float a, float b, float c, float alt, float anch, float prof, char type1) {
	x = a;
	y = b;
	z = c;
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	disList = -1;
	type = type1;
	exists = true;
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
char Plataforma::getType() {
	return type;
}
bool Plataforma::getExists() {
	return exists;
}
void Plataforma::setExists(bool exi) {
	exists = exi; 
}
void Plataforma::setPos(float a, float b, float c) {
	x = a;
	y = b;
	z = c;
}
void Plataforma::setType(char typ) {
	type = typ;
}
void Plataforma::draw(Vector3** modelo, int caras,GLuint textura) {
	glPushMatrix();
	glTranslatef(x, y, z);
	if(disList == -1) disList = drawFaces(modelo, caras, textura);
	if (disList != -1) glCallList(disList);
	glPopMatrix();
}