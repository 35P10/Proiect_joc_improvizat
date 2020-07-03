#define MFIL 19  // filas del mapa
#define MCOL 33

#include <iostream>
using namespace std;

class Mapa
{
    char**ptrMapa;
public:
    Mapa();
    Mapa(char[][MCOL]);
    char**getPtrMapa();
    void crearMatriz();
    void insertarMapa(char[][MCOL]); // recibe un mapa como una matriz estatica([][]) y lo asigna a ptrMapa como una matriz dinamica(**)
    void imprimirMapa();
};

Mapa::Mapa() {
    ptrMapa = nullptr;
}
Mapa::Mapa(char mapaEstatico[][MCOL]) {
    crearMatriz();
    insertarMapa(mapaEstatico);
}
char** Mapa::getPtrMapa() {
    return ptrMapa;
}
void Mapa::crearMatriz() {
    ptrMapa = new char*[MFIL];
    for(int i = 0; i < MFIL; i++)
        ptrMapa[i] = new char[MCOL];
}
void Mapa::insertarMapa(char mapaEstatico[][MCOL]) {
    for(int i = 0; i < MFIL; i++)
        for(int j = 0; j < MCOL; j++)
            ptrMapa[i][j] = mapaEstatico[i][j];
}
void Mapa::imprimirMapa() {
    for(int i = 0; i < MFIL; i++) {
        for(int j = 0; j < MCOL; j++)
            cout << ptrMapa[i][j];
        cout << endl;
    }
}
