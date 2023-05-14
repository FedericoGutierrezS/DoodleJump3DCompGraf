#pragma once
#include "mesh.h"
#include "Vector3.h"
#include "Particle.h"
#include <SDL/SDL_opengl.h>
#include <vector>

class ParticleEmitter {
private:
	Vector3* pos;
	Vector3* force;
	float dispersion;
	float size;
	bool exists;
	bool repeat;
	Vector3* color;
	Vector3* colorFade;
	float ALPHA;
	float alphaFade;
	float aliveTime;
	float gravity;
	int amount;
	Particle** particles;

public:
	ParticleEmitter(Vector3*, Vector3*, Vector3*, float = 1.0, Vector3* = new Vector3(0.0, 0.0, 0.0), float = 0.0, int = 5, float = 1.0, float = 0.0, float = 2.0, float = 9.8, bool = false, bool = true);
	~ParticleEmitter();
	void setGravity(float);
	float getGravity();
	void setExists(bool);
	bool getExist();
	void setDispersion(float);
	float getDispersion();
	void setSize(float = 1.0);
	float getSize();
	void setPos(Vector3*);
	Vector3* getPos();
	void setForce(Vector3*);
	Vector3* getForce();
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