#include <iostream>
#include "Punto.h"
using namespace std;
Punto::Punto() {
    x = 0; y = 0;
}
Punto::Punto(int x, int y) {
    this->x = x; this->y = y;
}
Punto::Punto(Punto &oPunto) {
    x = oPunto.x; y = oPunto.y;
}
int Punto::getX() {
    return x;
}
int Punto::getY() {
    return y;
}
void Punto::offset(int offsetX, int offsetY)
{
    x += offsetX; y += offsetY;
}
void Punto::imprimirPunto() {
    cout << "x = " << x << ", y = " << y << endl;
}
