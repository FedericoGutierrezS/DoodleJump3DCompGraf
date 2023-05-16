#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(Vector3* posVector, Vector3* velVector, Vector3* colorVector, float colorAlpha, Vector3* colorFadeVector, float fadeAlpha, int cantidad, float boxSize, float dispersionAmount, float time, Vector3* acc, bool existe, bool r)
{
	pos = posVector;
	vel = velVector;
	color = colorVector;
	ALPHA = colorAlpha;
	colorFade = colorFadeVector;
	alphaFade = fadeAlpha;
	size = boxSize;
	sizeFade = boxSize;
	dispersion = dispersionAmount;
	aliveTime = time;
	acceleration = acc;
	exists = existe;
	amount = cantidad;
	repeat = r;
	particles = new Particle*[cantidad];
	for (int i = 0; i < cantidad; i++) {
		particles[i] = nullptr;
	}
}

ParticleEmitter::~ParticleEmitter()
{
	for (int i = 0; i < amount; i++) {
		delete particles[i];
	}
	delete pos;
	delete vel;
	delete color;
	delete colorFade;
	delete acceleration;
}

void ParticleEmitter::setAcceleration(Vector3* acc)
{
	acceleration = acc;
}

Vector3* ParticleEmitter::getAcceleration()
{
	return acceleration;
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

void ParticleEmitter::setRepeat(bool b)
{
	repeat = b;
}

bool ParticleEmitter::getRepeat()
{
	return repeat;
}

void ParticleEmitter::draw(float x, float y, float z, float time)
{
	for (int i = 0; i < amount; i++)
	{
		if (particles[i] != nullptr && particles[i]->getTime() > particles[i]->getAliveTime()) {
			particles[i]->setExists(false);
			if (repeat && exists) {
				delete particles[i];
				particles[i] = nullptr;
			}
		}
		if (particles[i] == nullptr) {
			particles[i] = new Particle(time, new Vector3(pos->getX(), pos->getY(), pos->getZ()), new Vector3(color->getX(), color->getY(), color->getZ()), ALPHA, size, aliveTime * rand() / RAND_MAX, exists);
			particles[i]->setVel(new Vector3(vel->getX() + dispersion * (-1.0 + (2.0 * rand()) / (RAND_MAX + 1.0)), vel->getY(), vel->getZ() + dispersion * (-1.0 + (2.0 * rand()) / (RAND_MAX + 1.0))));
			particles[i]->setAcc(new Vector3(acceleration->getX(), acceleration->getY(), acceleration->getZ()));
		}
		if (particles[i]->getExist()) {
			particles[i]->incTime(time - particles[i]->getCreationTime() - particles[i]->getTime());
			float percent = (particles[i]->getTime()) / (particles[i]->getAliveTime());
			float xColor = color->getX() * (1 - percent) + colorFade->getX() * percent;
			float yColor = color->getY() * (1 - percent) + colorFade->getY() * percent;
			float zColor = color->getZ() * (1 - percent) + colorFade->getZ() * percent;
			float alphaDegrade = ALPHA * (1 - percent) + alphaFade * percent;
			particles[i]->setColor(xColor, yColor, zColor, alphaDegrade);
			particles[i]->draw(x, y, z);
		}
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
	delete pos;
	pos = new Vector3(position->getX(), position->getY(), position->getZ());
}

Vector3* ParticleEmitter::getPos()
{
	return pos;
}

void ParticleEmitter::setVelocity(Vector3* vector)
{
	vel = vector;
}

Vector3* ParticleEmitter::getVelocity()
{
	return vel;
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
	if (colorFade != NULL)
		delete colorFade;
	colorFade = new Vector3(r, g, b);
	alphaFade = a;
}

void ParticleEmitter::setFadeColor(Vector3* rgb, float a)
{
	if (colorFade != NULL)
		delete colorFade;
	colorFade = rgb;
	alphaFade = a;
}

Vector3* ParticleEmitter::getFadeColor()
{
	return colorFade;
}
