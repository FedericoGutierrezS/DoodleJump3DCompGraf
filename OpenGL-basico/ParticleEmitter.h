#pragma once
#include "mesh.h"
#include "Vector3.h"
#include "Particle.h"
#include <SDL/SDL_opengl.h>

class ParticleEmitter {
private:
	Vector3* pos;
	Vector3* vel;
	float dispersion;
	float size;
	float sizeFade;
	bool exists;
	bool repeat;
	Vector3* color;
	Vector3* colorFade;
	float ALPHA;
	float alphaFade;
	float aliveTime;
	Vector3* acceleration;
	int amount;
	Particle** particles;

public:
	ParticleEmitter(Vector3*, Vector3*, Vector3*, float = 1.0, Vector3* = new Vector3(0.0, 0.0, 0.0), float = 0.0, int = 5, float = 1.0, float = 0.0, float = 2.0, Vector3* = new Vector3(0.0, 9.8, 0.0), bool = false, bool = true);
	~ParticleEmitter();
	void setAcceleration(Vector3*);
	Vector3* getAcceleration();
	void setExists(bool);
	bool getExist();
	void setDispersion(float);
	float getDispersion();
	void setSize(float = 1.0);
	float getSize();
	void setPos(Vector3*);
	Vector3* getPos();
	void setVelocity(Vector3*);
	Vector3* getVelocity();
	void setColor(float, float, float, float = 1.0);
	void setColor(Vector3*, float = 1.0);
	Vector3* getColor();
	float getAlpha();
	void setFadeColor(float, float, float, float = 0.0);
	void setFadeColor(Vector3*, float = 0.0);
	Vector3* getFadeColor();
	void setAlphaFade(float);
	float getAlphaFade();
	void setAliveTime(float);
	float getAliveTime();
	void setRepeat(bool);
	bool getRepeat();
	void draw(float, float, float, float);
};