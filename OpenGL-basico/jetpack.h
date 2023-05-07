#include "mesh.h"
#include "Vector3.h"
#include <SDL/SDL_opengl.h>
#pragma once

class Jetpack {
private:
	float altCol;
	float anchCol;
	float profCol;
	Vector3* pos;
	int disList;
	bool exist;
	bool onPlayer;

public:
	Jetpack(float, float, float);
	float getAltCol();
	float getAnchCol();
	float getProfCol();
	bool getOnPlayer();
	bool getExist();
	Vector3* getPos();
	void setPos(float, float, float);
	void setPos(Vector3*);
	void draw(Vector3**, int, GLuint);
	void setExist(bool);
	void setOnPlayer(bool);
};