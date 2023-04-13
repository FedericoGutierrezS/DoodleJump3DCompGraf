#include "Vector3.h"
#include <math.h>
#include <iostream>

Vector3::Vector3() {
    x = 0;
    y = 0;
    z = 0;
    modulo = 0;
};

float Vector3::getX() {
    return x;
}

float Vector3::getY() {
    return y;
}

float Vector3::getZ() {
    return z;
}

float Vector3::getModulo() {
    return modulo;
}

float calculateModulo(Vector3 a) {
    return sqrt(pow(a.getX(), 2) + pow(a.getY(), 2) + pow(a.getZ(), 2));
}

Vector3::Vector3(float a, float b, float c) {
    this->x = a;
    this->y = b;
    this->z = c;
    modulo = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
};

Vector3* Vector3::suma(Vector3 a, Vector3 b) {
    Vector3* res = new Vector3(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
    return res;
};

Vector3* Vector3::resta(Vector3 a, Vector3 b) {
    Vector3* res = new Vector3(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
    return res;
};

Vector3* Vector3::multVecEsc(Vector3 a, float k) {
    Vector3* res = new Vector3(a.getX() * k, a.getY() * k, a.getZ() * k);
    return res;
};

Vector3* Vector3::cross(Vector3 a, Vector3 b) {
    Vector3* res = new Vector3(a.getY() * b.getZ() - a.getZ() * b.getY(), a.getX() * b.getZ() - a.getZ() * b.getX(), a.getX() * b.getY() - a.getY() * b.getX());
    return res;
};

float Vector3::dot(Vector3 a, Vector3 b) {
    return a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ();
};

float Vector3::angle(Vector3 a, Vector3 b) {
    return acos(dot(a, b) / (a.getModulo() * b.getModulo()));
};

Vector3* Vector3::negate(Vector3 a) {
    return new Vector3(-a.getX(), -a.getY(), -a.getZ());
};

Vector3* Vector3::divVecEsc(Vector3 a, float k) {
    return multVecEsc(a, 1 / k);
};

float Vector3::moduloSqr(Vector3 a) {
    return pow(a.getModulo(), 2);
};

Vector3* Vector3::normalize(Vector3 a) {
    return divVecEsc(a, a.getModulo());
};

Vector3* Vector3::symmetrical(Vector3 a, Vector3 b) {
    Vector3* norm = normalize(b);
    Vector3* mult2 = multVecEsc(*norm, 2);
    float dotProd = dot(*norm, a);
    Vector3* fact1 = multVecEsc(*mult2, dotProd);
    Vector3* res = resta(*fact1, a);
    delete norm;
    delete mult2;
    delete fact1;
    return res;
};

void Vector3::toString() {
    std::cout << "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]";
};