#include "bullet.h"

Bullet::Bullet(float a, float b, float c, float alt, float anch, float prof) {
	pos = new Vector3(a, b, c);
	exists = false;
	altCol = alt;
	anchCol = anch;
	profCol = prof;
	disList = -1;
}
Bullet::~Bullet() {
	delete pos;
}
float Bullet::getAltCol() {
	return altCol;
}
float Bullet::getAnchCol() {
	return anchCol;
}
float Bullet::getProfCol() {
	return profCol;
}
bool Bullet::getExists() {
	return exists;
}
void Bullet::setExists(bool exi) {
	exists = exi;
}
Vector3* Bullet::getPos() {
	return pos;
}
void Bullet::setPos(Vector3* npos) {
	delete pos;
	pos = npos;
}
void Bullet::draw(Vector3** modelo, int caras, GLuint textura) {
	if (disList == -1) disList = drawFaces(modelo, caras, textura);
	if (disList != -1) glCallList(disList);
}
