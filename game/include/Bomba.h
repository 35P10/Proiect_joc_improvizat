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

int IsThereABomb(Bomba *bombas,int max_bombas,int x, int y) //si en la coordenada x y (segun buffer) hay bomba
{
    for (int i=0;i<max_bombas;i++)
    {
        if(bombas[i].x==x && bombas[i].y==y){
            return 1;
            }
    }
    return 0;
}

void crearBomba(Bomba *bombas,Punto* coord,int &n_bomba,int max_bombas)
{
    if(bombas[n_bomba].contador==0){
    int cont=IsThereABomb(bombas,max_bombas,coord->getX(),coord->getY());
    if (cont==0){
    bombas[n_bomba].x=coord->getX();
    bombas[n_bomba].y=coord->getY();
    cout<<coord->getX()<<endl;
    cout<<coord->getY()<<endl;
    bombas[n_bomba].contador=1;
    n_bomba++;
    }
    }
}




void printBomba(BITMAP *bomba_bpm, BITMAP *buffer,Bomba *bombas,int n_bomba, int max_bombas)
{
    BITMAP *bomba_0 = load_bitmap("bomba0.bmp", NULL);
    for(int i=0;i<max_bombas;i++){
        if(bombas[i].contador==0)
        masked_blit(bomba_0,buffer,0, 0, bombas[i].x, bombas[i].y, 33, 33);
        else
        masked_blit(bomba_bpm,buffer,0, 0, bombas[i].x, bombas[i].y, 33, 33);
    }
}
