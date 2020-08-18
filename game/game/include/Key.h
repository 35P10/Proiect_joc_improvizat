#include "Jugador.h"

#include <iostream>
using namespace std;


class Key
{
    public:
    int x;
    int y;
    int status; //1=no recogida, 0=recogida

    Key()
    {
        x=0;
        y=0;
        status=1;
    }


    Key(int nivel, Mapa *arrayMapas)
    {
        int randomX;
        int randomY;
        srand((unsigned) time(0));
        randomX=(rand() % 30) + 1;
        randomY = (rand() % 17) + 1;
        while(true){
        if(arrayMapas[nivel].getPtrMapa()[randomY][randomX] != 'x' && arrayMapas[nivel].getPtrMapa()[randomY][randomX] != 't' && arrayMapas[nivel].getPtrMapa()[randomY][randomX] != 's')
        {
            x=randomX*45;
            y=randomY*45;
            status=1;
            break;
        }
        }
    }

    void setKey(int nivel, Mapa *arrayMapas)
    {
        int randomX;
        int randomY;
        srand((unsigned) time(0));
        randomX=(rand() % 30) + 1;
        randomY = (rand() % 17) + 1;
        while(true){
        if(arrayMapas[nivel].getPtrMapa()[randomY][randomX] != 'x' && arrayMapas[nivel].getPtrMapa()[randomY][randomX] != 't'&& arrayMapas[nivel].getPtrMapa()[randomY][randomX] != 's')
        {
            x=randomX*45;
            y=randomY*45;
            status=1;
            break;
        }
        else
        {
            randomX=(rand() % 30) + 1;
            randomY = (rand() % 17) + 1;
        }
        }
    }

    void checkStatus(Jugador *arrayJugadores, int cantJugadores)
    {
        for(int i=0 ; i<cantJugadores ; i++)
            {
                if (arrayJugadores[i].getCoordenada()->getX()== x && arrayJugadores[i].getCoordenada()->getY()==y){
                    x=0;
                    y=0;
                    arrayJugadores[i].setKey(1);
                    status = 0;
                }
            }
    }


    void printKey(BITMAP *buffer)
    {
        BITMAP *bomba_0 = load_bitmap("bomba0.bmp", NULL);
        BITMAP *key = load_bitmap("recursos/key.bmp", NULL);

        if(status==0)
            masked_blit(bomba_0,buffer,0, 0,  x, y, 45, 45);
        else
            masked_blit(key,buffer,0, 0, x,y, 45, 45);

}
};




