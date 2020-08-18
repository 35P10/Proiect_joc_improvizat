#include "Punto.h"


#include <iostream>
using namespace std;


class Bomba
{
    public:
    int x,y;
    int contador=0;
    Bomba()
    {
        x=0;
        y=0;
    }

    void SetBomba(Punto* coord)
    {
        x=coord->getX();
        y=coord->getY();
    }
};

int IsThereABomb(Bomba bombas[][5],int max_bombas,int x, int y) //si en la coordenada x y (segun buffer) hay bomba
{
    for(int j=0; j<2;j++){
    for (int i=0;i<max_bombas;i++)
    {
        if(bombas[j][i].x==x && bombas[j][i].y==y){
            return 1;
            }
    }
    }
    return 0;
}

void crearBomba(Bomba bombas[2][5],Punto* coord,int &n_bomba,int max_bombas,int n_jugador)
{
    if(bombas[n_jugador-1][n_bomba].contador==0){
        int cont=IsThereABomb(bombas,max_bombas,coord->getX(),coord->getY());
        if (cont==0){
            bombas[n_jugador-1][n_bomba].x=coord->getX();
            bombas[n_jugador-1][n_bomba].y=coord->getY();
            cout<<coord->getX()<<endl;
            cout<<coord->getY()<<endl;
            bombas[n_jugador-1][n_bomba].contador=1;
            n_bomba++;
        }
    }
}




void printBomba(BITMAP *bomba_bpm, BITMAP *buffer,Bomba bombas[2][5],int n_bomba, int max_bombas)
{
    BITMAP *bomba_0 = load_bitmap("bomba0.bmp", NULL);
    for (int j=0; j<2; j++)
    {
        for(int i=0; i<max_bombas; i++)
        {
            if(bombas[j][i].contador==0)
                masked_blit(bomba_0,buffer,0, 0, bombas[j][i].x, bombas[j][i].y, 45, 45);
            else
                masked_blit(bomba_bpm,buffer,0, 0, bombas[j][i].x, bombas[j][i].y, 45, 45);
        }
    }
}
