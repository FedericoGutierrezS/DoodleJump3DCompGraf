#pragma once
#include <string>

class Vector3 {
private:
    float x;
    float y;
    float z;
    float modulo;
public:
    Vector3();
    Vector3(float, float, float);
    float getX();
    float getY();
    float getZ();
    float getModulo();
    Vector3* suma(Vector3, Vector3);
    Vector3* resta(Vector3, Vector3);
    Vector3* multVecEsc(Vector3, float);
    Vector3* cross(Vector3, Vector3);
    float dot(Vector3, Vector3);
    float angle(Vector3, Vector3);
    Vector3* negate(Vector3);
    Vector3* divVecEsc(Vector3, float);
    float moduloSqr(Vector3);
    Vector3* normalize(Vector3);
    Vector3* symmetrical(Vector3, Vector3);
    void toString();
};