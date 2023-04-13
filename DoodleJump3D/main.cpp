#include <iostream>
#include <string>
#include <math.h>
#include "Vector3.h"

using namespace std;

int main() {
    float x;
    float y;
    float z;
    float delta;
    cin >> x;
    cin >> y;
    cin >> z;
    cin >> delta;

    if (z <= 0) cout << "[0,0,0]";
    else {
        Vector3 dummy;
        Vector3* pos = new Vector3();
        Vector3* vi = new Vector3(x, y, z);
        float tiempo = delta;
        Vector3* gravity = new Vector3(0, 0, -9.8f);
        pos = dummy.suma(*dummy.multVecEsc(*vi, tiempo), *dummy.multVecEsc(*gravity, (tiempo * tiempo) / 2));
        while (pos->getZ() > 0) {
            pos->toString();
            tiempo = tiempo + delta;
            pos = dummy.suma(*dummy.multVecEsc(*vi, tiempo), *dummy.multVecEsc(*gravity, tiempo * tiempo / 2));
        };

        cout << (z * z) / (2 * gravity->getModulo());
        cout << "    ";
        cout << tiempo - delta;
    };
};
