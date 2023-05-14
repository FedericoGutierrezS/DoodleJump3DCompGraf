#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(Vector3* posVector, Vector3* forceVector, Vector3* colorVector, float colorAlpha, Vector3* colorFadeVector, float fadeAlpha, int cantidad, float boxSize, float dispersionAmount, float time, float gravityAmount, bool existe)
{
	pos = posVector;
	force = forceVector;
	color = colorVector;
	ALPHA = colorAlpha;
	colorFade = colorFadeVector;
	alphaFade = fadeAlpha;
	size = boxSize;
	dispersion = dispersionAmount;
	aliveTime = time;
	gravity = gravityAmount;
	exists = existe;
	amount = cantidad;
	particles = new Particle*[cantidad];
	for (int i = 0; i < cantidad; i++) {
		particles[i] = nullptr;
	}
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::setGravity(float g)
{
	gravity = g;
}

float ParticleEmitter::getGravity()
{
	return gravity;
}

void ParticleEmitter::setExists(bool e)
{
	exists = e;
}

bool ParticleEmitter::getExist()
{
	return exists;
}

void ParticleEmitter::setAlphaFade(float alpha)
{
	alphaFade = alpha;
}

float ParticleEmitter::getAlphaFade()
{
	return alphaFade;
}

void ParticleEmitter::setAliveTime(float time)
{
	aliveTime = time;
}

float ParticleEmitter::getAliveTime()
{
	return aliveTime;
}

void ParticleEmitter::draw(float x, float y, float z, float time)
{
	for (int i = 0; i < amount; i++)
	{
		if (particles[i] != nullptr && particles[i]->getTime() > particles[i]->getAliveTime()) {
			//particles[i]->~Particle();
			delete particles[i];
			particles[i] = nullptr;
		}
		if (particles[i] == nullptr) {
			particles[i] = new Particle(time, new Vector3(pos->getX(), pos->getY(), pos->getZ()), new Vector3(color->getX(), color->getY(), color->getZ()), ALPHA, size, aliveTime * rand() / RAND_MAX, exists);
			particles[i]->setVel(new Vector3(force->getX() + dispersion * (-1.0 + (2.0 * rand()) / (RAND_MAX + 1.0)), force->getY(), force->getZ() + dispersion * (-1.0 + (2.0 * rand()) / (RAND_MAX + 1.0))));
			particles[i]->setAcc(new Vector3(0, gravity, 0));
		}
		particles[i]->incTime(time - particles[i]->getCreationTime() - particles[i]->getTime());
		particles[i]->draw(x, y, z);
	}
}

void ParticleEmitter::setDispersion(float d)
{
	dispersion = d;
}

float ParticleEmitter::getDispersion()
{
	return dispersion;
}

void ParticleEmitter::setSize(float s)
{
	size = s;
}

float ParticleEmitter::getSize()
{
	return size;
}

void ParticleEmitter::setPos(Vector3* position)
{
	pos = position;
}

Vector3* ParticleEmitter::getPos()
{
	return pos;
}

void ParticleEmitter::setForce(Vector3* vector)
{
	force = vector;
}

Vector3* ParticleEmitter::getForce()
{
	return force;
}

void ParticleEmitter::setColor(float r, float g, float b, float a)
{
	if (color != NULL)
		delete color;
	color = new Vector3(r, g, b);
	ALPHA = a;
}

void ParticleEmitter::setColor(Vector3* RGB, float a)
{
	if (color != NULL)
		delete color;
	color = RGB;
	ALPHA = a;
}

Vector3* ParticleEmitter::getColor()
{
	return color;
}

float ParticleEmitter::getAlpha()
{
	return ALPHA;
}

void ParticleEmitter::setFadeColor(float r, float g, float b, float a)
{
	colorFade = new Vector3(r, g, b);
	alphaFade = a;
}

void ParticleEmitter::setFadeColor(Vector3* rgb, float a)
{
	colorFade = rgb;
	alphaFade = a;
}

Vector3* ParticleEmitter::getFadeColor()
{
	return colorFade;
}
