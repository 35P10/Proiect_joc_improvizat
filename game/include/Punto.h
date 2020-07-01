#ifndef PUNTO_H
#define PUNTO_H


class Punto
{
public:
    int x, y;
    Punto();
    Punto(int, int);
    Punto(Punto &);
    int getX();
    int getY();
    void offset(int, int);
    void imprimirPunto();
};

#endif // PUNTO_H
