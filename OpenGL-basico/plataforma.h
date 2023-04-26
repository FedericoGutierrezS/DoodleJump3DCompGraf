#include "mesh.h"
#include "Vector3.h"
#include <SDL/SDL_opengl.h>
#pragma once


class Plataforma {
private:
	float x;
	float y;
	float z;
	float altCol;
	float anchCol;
	float profCol;

public:
	Plataforma(float, float, float, float, float, float);
	float getX();
	float getY();
	float getZ();
	float getAltCol();
	float getAnchCol();
	float getProfCol();
	void draw(Vector3**, int, GLuint);
};