#include <iostream>
#include <ctime>
using namespace std;



class PowerUp
{
    public:
    int x=0;
    int y=0;
    int contador=0;
    PowerUp()
    {
        x=0;
        y=0;
        contador=0;
    }

    void checkStatus(Jugador *arrayJugadores, int cantJugadores,int tiempo_powerUp, int max_bombas)
    {
        if(contador!=0){
            contador++;
            if(contador==tiempo_powerUp)
            {
                x=0;
                y=0;
                contador=0;
                //cout<<"FinDePowerUp";
            }

            for(int i=0 ; i<cantJugadores ; i++)
            {
                if (arrayJugadores[i].getCoordenada()->getX()== x && arrayJugadores[i].getCoordenada()->getY()==y){

                    srand((unsigned) time(0));
                    x=(rand() % 3) + 1;

                    if (x==1)
                    {
                        if (arrayJugadores[i].getAlcanzeDeBomba()<4)
                        {
                        cout<<"Alcanze de Bomba +1: ";
                        arrayJugadores[i].addAlcanzeDeBomba(1);
                        cout<<arrayJugadores[i].getAlcanzeDeBomba()<<endl;
                        }
                    }
                    else if (x==2)
                    {
                        if (arrayJugadores[i].getSalud()<9)
                        {
                        cout<<"Salud +1";
                        arrayJugadores[i].addSalud(1);
                        }
                    }
                    else if (x==3)
                    {
                        if (arrayJugadores[i].getCantBombas()<max_bombas)
                        {
                        cout<<"nBombas +1";
                        arrayJugadores[i].addCantBombas(1);}
                    }

                    x=0;
                    y=0;
                    contador=0;

                }
            }

        }



    }


};


void printPowerUp(BITMAP *bomba_bpm, BITMAP *buffer, PowerUp *PowerUps,int max_powerUps)
{
    BITMAP *bomba_0 = load_bitmap("bomba0.bmp", NULL);

    for(int i=0;i<max_powerUps;i++){
        //cout<<"i:" <<i<<endl;
        //cout<<"x:" <<PowerUps[i].x<<endl;
        //cout<<"y: "<<PowerUps[i].y<<endl;
        if(PowerUps[i].contador==0)
        masked_blit(bomba_0,buffer,0, 0,  PowerUps[i].x, PowerUps[i].y, 45, 45);
        else
        masked_blit(bomba_bpm,buffer,0, 0, PowerUps[i].x, PowerUps[i].y, 45, 45);
    }
}

