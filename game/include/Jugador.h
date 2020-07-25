#include "Punto.h"
#include <iostream>
using namespace std;



class Jugador
{
    Punto *coordenada;
    int salud;
    int velocidad;
    int contador; //de velocidad
    int cantBombas;
    int llave;
    int alcanzeDeBomba;
    void init();
public:
    Jugador();
    Jugador(Punto);

    Punto *getCoordenada();
    void plantarBomba();
    void imprimirDatos();
    void addSalud(int x);
    int getSalud();
    void movimiento(int nivel, Mapa *arrayMapas, Bomba *bombas,int max_bombas,int &n_bomba);
    void setKey(int x);
    int getKey();
    void addAlcanzeDeBomba(int x);
    int getAlcanzeDeBomba();
    int checkStatus();
    void addCantBombas(int x);
    int getCantBombas();
};

int Jugador::checkStatus()
{
    if (salud<=0)
        return 1;
    else
        return 0;
}


int Jugador::getAlcanzeDeBomba()
{
    return alcanzeDeBomba;
}

void Jugador::addAlcanzeDeBomba(int x)
{
    alcanzeDeBomba=alcanzeDeBomba+ x;
}

void Jugador::addCantBombas(int x)
{
    cantBombas=cantBombas+ x;
}

int Jugador::getCantBombas()
{
    return cantBombas;
}

int Jugador::getSalud()
{
    return salud;
}

void Jugador::init() {
    salud      = 1;
    velocidad  = 3;
    cantBombas = 1;
    contador = 0;
    llave=0; //0-> no tiene / 1-si la tiene
    alcanzeDeBomba=1;
    coordenada = new Punto(0,0);
}
Jugador::Jugador() {
    init();
}
Jugador::Jugador(Punto coord) {
    init();
    *coordenada = coord;          // usa ctor. copia de Punto
}
Punto* Jugador::getCoordenada() {
    return coordenada;
}

void Jugador::imprimirDatos() {
    coordenada->imprimirPunto();
    cout << salud << endl;
    cout << velocidad << endl;
    cout << cantBombas << endl;
}

void Jugador::addSalud(int x)
{
    salud = salud + x;
}

void Jugador::setKey(int x)
{
    if (x==0 || x==1)
        llave=x;
}

int Jugador:: getKey()
{
    return llave;
}

void Jugador::movimiento(int nivel, Mapa *arrayMapas,Bomba *bombas,int max_bombas,int &n_bomba)
{
    if (contador>0)
        contador--;
    if (contador==0)
    {
    int Y=coordenada->getY()/45;
    int X=coordenada->getX()/45;
    int y1=Y-1;
    int y2=Y+1;
    int x1=X-1;
    int x2=X+1;
    //cout << salud << endl;
    if(arrayMapas[nivel].getPtrMapa()[y1][X] == 'k' && key[KEY_W] && y1>-1 && IsThereABomb(bombas,max_bombas,X*45,y1*45)==0)
        coordenada->offset(0,-45);
    if(arrayMapas[nivel].getPtrMapa()[y2][X] == 'k' && key[KEY_S]&& y2<18 && IsThereABomb(bombas,max_bombas,X*45,y2*45)==0)
        coordenada->offset(0,+45);
    if(arrayMapas[nivel].getPtrMapa()[Y][x1] == 'k' && key[KEY_A] && x1>-1 && IsThereABomb(bombas,max_bombas,x1*45,Y*45)==0)
        coordenada->offset(-45,0);
    if(arrayMapas[nivel].getPtrMapa()[Y][x2] == 'k' && key[KEY_D] && x2<32 && IsThereABomb(bombas,max_bombas,x2*45,Y*45)==0)
        coordenada->offset(+45,0);

    contador=velocidad;
    }

    if (key[KEY_E]) {
            crearBomba(bombas,coordenada,n_bomba,cantBombas);
            if (n_bomba==cantBombas)
                n_bomba=0;
    }

}


