#pragma once
#include "jugador.h"

class Enemigo : public Jugador {
private:
	float enemyCenter;
public:
	Enemigo(float x, float y, float z) :Jugador(x, y, z) {

	};
	float getEnemyCenter() { return enemyCenter; };
	void setPos(float x, float y, float z) {
		getPos()->setX(x);
		getPos()->setY(y);
		getPos()->setZ(z);
		enemyCenter = x;
	}
};