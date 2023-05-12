#pragma once
#include "mesh.h"
#include "Vector3.h"
#include <SDL/SDL_opengl.h>

class Bullet {
private:
	float altCol;
	float anchCol;
	float profCol;
	int disList;
	bool exists;
	Vector3* pos;

public:
	Bullet(float, float, float, float, float, float);
	~Bullet();
	Vector3* getPos();
	bool getExists();
	float getAltCol();
	float getAnchCol();
	float getProfCol();
	void setExists(bool);
	void setPos(Vector3*);
	void draw(Vector3**, int, GLuint);
};
