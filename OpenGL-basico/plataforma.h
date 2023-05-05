#include "mesh.h"
#include "Vector3.h"
#include <SDL/SDL_opengl.h>
#pragma once


class Plataforma {
protected:
	float x;
	float y;
	float z;
private:
	float altCol;
	float anchCol;
	float profCol;
	int disList;
	char type;
	bool exists;

public:
	Plataforma(float, float, float, float, float, float, char);
	float getX();
	float getY();
	float getZ();
	bool getExists();
	char getType();
	float getAltCol();
	float getAnchCol();
	float getProfCol();
	void setExists(bool);
	void setPos(float, float, float);
	void draw(Vector3**, int, GLuint);
};