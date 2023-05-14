#pragma once
#include "mesh.h"
#include "Vector3.h"
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>

class Particle {
private:
	Vector3* pos;
	Vector3* realPos;
	float size;
	bool exists;
	Vector3* color;
	float ALPHA;
	int disList;
	float aliveTime;
	float time;
	Vector3* vel;
	Vector3* acc;
	float creationTime;

public:
	Particle(float, Vector3*, Vector3* = new Vector3(1,1,1), float = 1.0, float = 1.0, float = 2.0, bool = false, float = 0.0);
	~Particle();
	void setExists(bool);
	bool getExist();
	void setSize(float = 1.0);
	float getSize();
	void setPos(Vector3*);
	void setPos(float, float, float);
	Vector3* getPos();
	void setColor(float, float, float, float = 1.0);
	void setColor(Vector3*, float = 1.0);
	Vector3* getColor();
	float getAlpha();
	void setAliveTime(float);
	float getAliveTime();
	Vector3* getVel();
	void setVel(Vector3*);
	void setAcc(Vector3*);
	void draw(float, float, float);
	float getTime();
	float getCreationTime();
	void incTime(float = 1.0);
};