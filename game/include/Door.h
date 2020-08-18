#include <iostream>

class Door
{
    public:
    int x;
    int y;
    int status;

    Door(int newX,int newY)
    {
        x = newX;
        y= newY;
        status=0;
    }

    void setDoor(int newX,int newY)
    {
        x = newX;
        y= newY;
    }

    void setStatus(int X)
    {
        status=X;
    }

    int checkStatus(Jugador *arrayJugadores, int cantJugadores)
    {
        for(int i=0 ; i<cantJugadores ; i++)
            {
                if (arrayJugadores[i].getCoordenada()->getX()== x && arrayJugadores[i].getCoordenada()->getY()==y && arrayJugadores[i].getKey()==1){
                    status = 1;
                    return 1;
                }
            }
        return 0;
    }

    void printDoor(BITMAP *buffer,Jugador *arrayJugadores)
    {
        BITMAP *door = load_bitmap("recursos/door.bmp", NULL);
        BITMAP *door2 = load_bitmap("recursos/door2.bmp", NULL);
        if(status==0)
        {
            if(arrayJugadores[0].getKey()==0)
                masked_blit(door,buffer,0, 0,  x, y, 45, 45);
            else
                masked_blit(door2,buffer,0, 0,  x, y, 45, 45);
        }


    }
};


