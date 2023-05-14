#include "Particle.h"

Particle::Particle(Vector3* posVector, Vector3* rgb, float a, float s, float at, bool e)
{
	pos = posVector;
	color = rgb;
	ALPHA = a;
	size = s;
	aliveTime = at;
	exists = e;
	time = 0;
	vel = new Vector3(0, 0, 0);
	disList = -1;
}

Particle::~Particle()
{
	delete color;
	delete vel;
	delete pos;
}

void Particle::setExists(bool e)
{
	exists = e;
}

bool Particle::getExist()
{
	return exists;
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
	color = new Vector3(r, g, b);
	ALPHA = a;
}

void Particle::setColor(Vector3* rgb, float a)
{
	color = rgb;
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

	void Particle::draw(float x, float y, float z)
	{
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glTranslatef(pos->getX(), pos->getY(), pos->getZ());

		// Calculate the vector from the particle to the camera
		float cameraVectorX = x - pos->getX();
		float cameraVectorY = y - pos->getY();
		float cameraVectorZ = z - pos->getZ();

		// Calculate the angle between the camera vector and the z-axis
		float angleXZ = atan2(cameraVectorX, cameraVectorZ);
		float angleYZ = atan2(cameraVectorY, cameraVectorZ);

		// Apply rotations to make the square face the camera
		glRotatef(angleXZ * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
		glRotatef(-angleYZ * 180.0f / M_PI, 1.0f, 0.0f, 0.0f);

		if (disList == -1) {
			int displayList = glGenLists(1);
			glNewList(displayList, GL_COMPILE);

			//glDepthMask(GL_FALSE);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(color->getX(), color->getY(), color->getZ(), ALPHA);
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
			//glDepthMask(GL_TRUE);

			glEndList();
		
			disList = displayList;
		}
		else {
			glCallList(disList);
		}
		glPopAttrib();
		glPopMatrix();
	}
