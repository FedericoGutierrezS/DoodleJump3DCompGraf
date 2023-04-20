// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cmath>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include < windows.h >

using namespace std;

class Vector3 {
private:
    float x, y, z;


public:
    Vector3() {
        x = 0;
        y = 0;
        z = 0;
    };

    Vector3(float x1, float y1, float z1) {
        x = x1;
        y = y1;
        z = z1;
    }

    Vector3 vectorSum(Vector3 v1, Vector3 v2) {
        return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    Vector3 vectorRest(Vector3 v1, Vector3 v2) {
        return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    Vector3 vectorByLambda(Vector3 v1, float x) {
        return Vector3(v1.x * x, v1.y * x, v1.z * x);
    }

    void movimientoProyectil(Vector3 velocidadInicial, int t) {
        float x0 = velocidadInicial.x;
        float y0 = velocidadInicial.y;
        float z0 = velocidadInicial.z;

        x = 0 + (x0 * t);
        y = 0 + ((y0 * t) - (0.5 * 9.8 * t * t));
        z = 0 + (z0 * t);
    }

    

    std::string print() {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }


};

int main()
{
    int opperation;
    std::cout << "Ingresa operacion\n";
    std::cin >> opperation;
    float x, y, z;
    switch (opperation) {
        
        case 1:
        {
            std::cout << "Ingresa primer vector\n";
            std::cin >> x >> y >> z;
            Vector3 v1 = Vector3(x, y, z);

            float x2, y2, z2;
            std::cout << "Ingresa el segundo vector\n";
            std::cin >> x2 >> y2 >> z2;
            Vector3 v2 = Vector3(x2, y2, z2);

            Vector3 vector;

            Vector3 v3 = vector.vectorSum(v1, v2);
            std::cout << "v1:  " << v1.print();
            std::cout << "v2:  " << v2.print();
            std::cout << "El resultado es  " << v3.print();
            break;
        }
            
        case 2:
        {
            std::cout << "Ingresa el vector de velocidad inicial\n";
            std::cin >> x >> y >> z;
            Vector3 velocidadInicial = Vector3(x, y, z);
            int deltaT;
            float elapsedT;
            std::cout << "Ingresa intervalo en segundos\n";
            std::cin >> deltaT;
            elapsedT = deltaT;
            Vector3 posicion;
            while (true) {
                // Ejecutar la función
                std::cout << "Current Time" << std::to_string(elapsedT);
                posicion.movimientoProyectil(velocidadInicial, elapsedT);
                elapsedT += deltaT;
                std::cout << posicion.print() << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(deltaT));
            }
            break;
        }
    }
}
