#include "Particle.h"

Particle::Particle(float ct, Vector3* posVector, Vector3* rgb, float a, float s, float at, bool e, float t)
{
	pos = posVector;
	realPos = new Vector3(posVector->getX(), posVector->getY(), posVector->getZ());
	color = new Vector3(rgb->getX(), rgb->getY(), rgb->getZ());
	ALPHA = a;
	size = s;
	aliveTime = at;
	exists = e;
	time = t;
	vel = new Vector3(0, 0, 0);
	disList = -1;
	acc = new Vector3(0, 0, 0);
	creationTime = ct;
}

Particle::~Particle()
{
	delete color;
	delete vel;
	delete pos;
	delete realPos;
	delete acc;
}

void Particle::setExists(bool e)
{
	exists = e;
}

bool Particle::getExist()
{
	return exists;
}

float Particle::getTime()
{
	return time;
}

float Particle::getCreationTime()
{
	return creationTime;
}

void Particle::incTime(float t) {
	time = time + t;
}

void Particle::setSize(float s)
{
	size = s;
}

float Particle::getSize()
{
	return size;
}

void Particle::setPos(Vector3* p)
{
	pos = p;
}

void Particle::setPos(float x, float y, float z)
{
	pos->setX(x);
	pos->setY(y);
	pos->setZ(z);
}

Vector3* Particle::getPos()
{
	return pos;
}

void Particle::setColor(float r, float g, float b, float a)
{
	color->setX(r);
	color->setY(g);
	color->setZ(b);
	ALPHA = a;
}

void Particle::setColor(Vector3* rgb, float a)
{
	color->setX(rgb->getX());
	color->setY(rgb->getY());
	color->setZ(rgb->getZ());
	ALPHA = a;
}

Vector3* Particle::getColor()
{
	return color;
}

float Particle::getAlpha()
{
	return ALPHA;
}

void Particle::setAliveTime(float at)
{
	aliveTime = at;
}

float Particle::getAliveTime()
{
	return aliveTime;
}

Vector3* Particle::getVel() {
	return vel;
}

void Particle::setVel(Vector3* vect) {
	vel = vect;
}

void Particle::setAcc(Vector3* a)
{
	acc = a;
}

void Particle::draw(float x, float y, float z)
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	float distanceX = vel->getX() * time + 0.5 * acc->getX() * time * time;
	float distanceY = vel->getY() * time + 0.5 * acc->getY() * time * time;
	float distanceZ = vel->getZ() * time + 0.5 * acc->getZ() * time * time;
	realPos->setX(pos->getX() + distanceX);
	realPos->setY(pos->getY() + distanceY);
	realPos->setZ(pos->getZ() + distanceZ);
	glTranslatef(realPos->getX(), realPos->getY(), realPos->getZ());

	// Calcula el vector de la particula a la camara
	float cameraVectorX = x - realPos->getX();
	float cameraVectorY = y - realPos->getY();
	float cameraVectorZ = z - realPos->getZ();

	float angleXZ = atan2(cameraVectorX, cameraVectorZ);
	float angleYZ = atan2(cameraVectorY, cameraVectorZ);

	// Aplica rotaciones para que la particula mire la camara
	glRotatef(angleXZ * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
	glRotatef(angleYZ * 180.0f / M_PI, 1.0f, 0.0f, 0.0f);

	glColor4f(color->getX(), color->getY(), color->getZ(), ALPHA);

	if (disList == -1) {
		int displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glNormal3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex3f(-0.5 * size, -0.5 * size, 0.0);
		glVertex3f(0.5 * size, -0.5 * size, 0.0);
		glVertex3f(0.5 * size, 0.5 * size, 0.0);
		glVertex3f(-0.5 * size, 0.5 * size, 0.0);
		glEnd();

		glColor4f(1, 1, 1, 1);
		glDisable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		glEndList();
		
		disList = displayList;
	}
	else {
		glCallList(disList);
	}
	glPopAttrib();
	glPopMatrix();
}
