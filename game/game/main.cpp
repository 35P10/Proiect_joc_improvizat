#include <allegro.h>
#define MFIL 20  // filas del mapa
#define MCOL 33  // columnas del mapa, en realidad seran 20(o sea -1) descontando el caracter final '\0' (//solo puede ser impar?)
#include "Punto.h"
#include "Mapa.h"
#include "Bomba.h"

#include "Key.h"
#include "PowerUp.h"
#include "Door.h"

#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

// == CLASE JUEGO =================================================================

class Juego
{
    BITMAP *buffer;         // buffer: se asigna su valor en el main, buffer es el espacio donde se insertan los sprites(imagenes)
    BITMAP *buffer2;
    BITMAP *buffer3;
    BITMAP *buffer4;        //buffer auxiliar
    BITMAP *muro;           // muro: tambien se asigna en el main, es la variable que va a contener la imagen con q se hacen los muros
    BITMAP *muro2;
    BITMAP *muro3;
    BITMAP *estadisticas;
    BITMAP *player;         // player: buffer de personaje
    BITMAP *player_bmp;     // player_bmp: imagen bmp del personaje
    BITMAP *bomba_bmp;
    BITMAP *muro_destructible;
    BITMAP *muro_destructible2;

    int unit = 45;          // tamaño de cada unidad de imagen en el mapa  (en pixeles)
    int cantMapas;   // cantidad de niveles
    Mapa *arrayMapas;       // array con punteros a cada objeto Mapa (niveles)

    int cantJugadores;
    Jugador *arrayJugadores;
    int dir = 0;

    Bomba bombas[2][5];
    int max_bombas = 5;

    PowerUp PowerUps[3];
    int max_powerUps = 3;
    int tiempo_powerUp = 300;


public:
    Juego(int op); //n de jugadores
    ~Juego();
    Mapa* getArrayMapas();
    Jugador* getArrayJugadores();



    //==Mapas
    void crearArrayMapas(); //creacion de los mapas
    void dibujarMapa(int,int); //en el buffer
    //==Jugadores
    void crearArrayJugadores(); //creacion de los jugadores
    void dibujarJugador(int); //en el buffer
    //==Bombas
    void explosionDeBomba(int tiempo,int n_mapa,int alcanze,int n_jugador);
    void collisionBomba(int X,int Y,int nivel,int alcanze);
    //==PowerUps
    void powerUpGenerator (int &n_powerUp, int nivel, int &t_generacionDePowerUp); //inserta los power ups
    void ejecucionPowerUps(); //contador de tiempo en el buffer y aumento de caracteristicas
    //==EjecucionDelJuego
    void nextLevel(int &n_mapa,Key &theKey, Door &theDoor);
    int pause();
    void gameover(int &status,int perdedor);
    void checkSaludPlayers(int &status);
    void imprimirStadisticas();
    void mostrarPantalla();
    void ejecutar();
};



Juego::Juego(int op)
{
    buffer     = create_bitmap((MCOL-1)*45, MFIL*45); // *30 ->es el tam de la imagen (30px x 30px)
    buffer2     = create_bitmap(76, MFIL*45);
    buffer3     = create_bitmap(76, MFIL*45);
    buffer4     = create_bitmap((MCOL-1)*45-76, MFIL*45);
    muro       = load_bitmap("muro.bmp", NULL);
    muro2       = load_bitmap("muro2.bmp", NULL);
    player     = create_bitmap(45, 90);  // ver si necesita mas pixeles
    estadisticas = load_bitmap("recursos/estadisticas.bmp", NULL);
    player_bmp = load_bitmap("player.bmp", NULL);
    muro_destructible  = load_bitmap("muro_destructible.bmp", NULL);
    bomba_bmp  = load_bitmap("bomba.bmp", NULL);

    cantMapas  = 6; // serán más
    arrayMapas = new Mapa[cantMapas];
    crearArrayMapas();
    cantJugadores  = op; //
    arrayJugadores = new Jugador[cantJugadores];
    crearArrayJugadores();
}

Juego::~Juego()
{

    delete []buffer;         // buffer: se asigna su valor en el main, buffer es el espacio donde se insertan los sprites(imagenes)
    delete []buffer2;
    delete []buffer3;
    delete []buffer4;
    delete []muro;           // muro: tambien se asigna en el main, es la variable que va a contener la imagen con q se hacen los muros
    delete []player;         // player: buffer de personaje
    delete []player_bmp;     // player_bmp: imagen bmp del personaje
    delete []bomba_bmp;
    delete []muro_destructible;
    delete []arrayMapas;
    delete []arrayJugadores;
}

Mapa* Juego::getArrayMapas() {
    return arrayMapas;
}


Jugador* Juego::getArrayJugadores() {
    return arrayJugadores;
}

void Juego::crearArrayMapas()
{
    char mapa_00[MFIL][MCOL] = { // NIVEL 0
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //NIVEL 0 multi
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx", //bloques destructibles = y
	"xkkkkkkkkkkkkkyyyykkkkkkkkkkkkkx", //bloques indestructibles = t & x & s
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx", //alto =17, ancho=20
	"xkkkkkkkkkkkkkyyyykkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkyyyykkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkyyyykkkkkkkkkkkkkx",
	"xkkkkskkkkkkkkkkkkkkkkkkkkskkkkx",
	"xkkkksykkkkkkkkkkkkkkkkkkyskkkkx",
	"xkkkkssyyyyyyyyyyyyyyyyyysskkkkx",
	"xkkkksykkkkkkkkkkkkkkkkkkyskkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
	"xttttttttttttttttttttttttttttttx"
    };                              // "mapa_00" es el diseño del mapa hecho con chars
    Mapa oMapa_00;         // se crea "oMapa_00" que es un objeto de la clase Mapa, basado en el diseño de "mapa_00"
    oMapa_00.getPtrMapa(mapa_00);
    arrayMapas[0] = oMapa_00;       // se inserta el objeto "oMapa_00" en la primera posicion del array de mapas del juego

    char mapa_01[MFIL][MCOL] = { // NIVEL 1
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //alto =19, ancho=32
    "xkkkykysyykykxyyyxkkkxyykkkykkyx", //NIVEL 1
    "xkssysysysyxysksysysksksysysyskx",
    "xkkyykkskykkksksysksyskykkkyyykx",
    "xksyssysksssysysysyskyksysysyskx",
    "xyyyykksykkyksyskskssskyyyykkykx",
    "xkssysysysysysksysysssksysysyskx",
    "xkkyykkskykyksyskskskykkykkykkyx",
    "xyssysysksssksksksyssssssssssskx",
    "xkkkykksykkyysyskskskkykkyyykkkx",
    "xksyssysksysysksysysyssssssssssx",
    "xykkykkskyykksysysyskkyykkkyykkx",
    "xkssksysysssyskskskssssssssssskx",
    "xykkykksyyykksksksyskkkyykkkyykx",
    "xkssysysksysksksyskskssssssssssx",
    "xkkkkyysykkyysyskskskyyskkkskkkx",
    "xysyssysksssksysysysysksysysysyx",
    "xyyyyyykkkykkykskykskskykskyyskx",
    "xttttttttttttttttttttttttttttttx"
    };
    Mapa oMapa_01;
    oMapa_01.getPtrMapa(mapa_01);
    arrayMapas[1] = oMapa_01;

    char mapa_02[MFIL][MCOL] = { // NIVEL 2
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //nivel 2
	"xkskyyskssyyyxksskkykykkskskykyx",
	"xksksysykyksksyyssksksykyskkssyx",
	"xkssskskssysykskysyykykssksksykx",
	"xkkykyskykysksyyssysysykkskykskx",
	"xysssysyssysksyyskykkkyssysyskkx",
	"xkskykskykksyksyysksyskkyskykskx",
	"xysksysysskskskyskkykkyssysyskkx",
	"xksyskskkkksksykysysysykysykyskx",
	"xysksysysskskksyskkkkykssysysyyx",
	"xksyskskkkksyssyssysksykyskykskx",
	"xkyksyssksysksykyskyksyssysksyyx",
	"xyskyksksskskssskyksykkykkyyyskx",
	"xksksyskyyksykkysskyksssysssysyx",
	"xksyskskykkskkykssykysssysssyskx",
	"xyskskssksykkyykykksykkkyykykskx",
	"xkyksykkysysksssksssksksksysksKx",
	"xttttttttttttttttttttttttttttttx"
    };
    Mapa oMapa_02;
    oMapa_02.getPtrMapa(mapa_02);
    arrayMapas[2] = oMapa_02;

    char mapa_03[MFIL][MCOL] = { // NIVEL 3
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", // NIVEL 3
	"xkkkkykkysssykyykykyksykkykkyksx",
	"xkskykskssskyssskyssskyskksysksx",
	"xysyyssyyssysyysyykkykskkksyskkx",
	"xyysyyysysyysysyysssyyyysykksskx",
	"xysssssyyyysyysysyyyyssssksykkyx",
	"xyyyyyyysysyyssysyssysssyksksykx",
	"xyssyssysysysyyyysssykykksykskkx",
	"xyysyyysssyyyysskkykskssyskssykx",
	"xsksysyyyyyssysskkssykssysysskyx",
	"xsyysyyysysssykksksyyskykkyssykx",
	"xkysskssssskyskysysysssssyssskkx",
	"xysskykykykykyksskkkykkksysskykx",
	"xyssskyssssssssssssssssysysyksyx",
	"xykkysksyssyyysyyysyyyyyyykksskx",
	"xysyssyssksysyyysyyysysysssssskx",
	"xssksskssysykyxkkysykyskykykskkx",
	"xykykkyykkykskkysykkskssssskykkx",
	"xttttttttttttttttttttttttttttttx"
    };
    Mapa oMapa_03;
    oMapa_03.getPtrMapa(mapa_03);
    arrayMapas[3] = oMapa_03;

    char mapa_04[MFIL][MCOL] = { // NIVEL 4
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //NIVEL 4
	"xkkyyyyyyyyyssyssssyyyysssyysyyx",
	"xkyssssysyssysyyyssysysyyysyssyx",
	"xsysyyyssyyyyyysyyyysysysyyyyyyx",
	"xyysssyyyyssysysyssysysysysyssyx",
	"xysyyyssyssyyyyyysyysyyyyysyssyx",
	"xysysysyysyssssssssyyysysysysyyx",
	"xyyysysyssysyyysyyyysysyyyyyyyyx",
	"xsssyysyysysysyyyssyssssssssssyx",
	"xyyyysssyyyyyssyyysyyyyyyyyyyyyx",
	"xyssysssssssyyyysysysssysysyssyx",
	"xysyyyyyysysyssyyyyysyssyysyysyx",
	"xysyyyssysyyyssssssssyyyysyyskkx",
	"xysysyyyyyysysssyyyssysssyyyssyx",
	"xysysysyssysyyyyssysyyyysssyyskx",
	"xyyyyysysyyssysyyyyyysskyyksyskx",
	"xyssyssyssyssysysysssyssysykkskx",
	"xsxyysyyysyyyyyyyyyyyyyyyssskkkx",
	"xttttttttttttttttttttttttttttttx"
    };
    Mapa oMapa_04;
    oMapa_04.getPtrMapa(mapa_04);
    arrayMapas[4] = oMapa_04;

    char mapa_05[MFIL][MCOL] = { // NIVEL 5
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //NIVEL 5
	"xkkksyyyyyyyyyyyyyyyyysyyyssyyyx",
	"xkskyksyyysssyyssssysyyssysyysyx",
	"xkssyssysyyyyysyyyyyssyyyysyssyx",
	"xkykkykykksssysysysysysssssyssyx",
	"xyysyysssyyyyyyyysyyyyyyysyyssyx",
	"xyyysyssssssyysyysyysysyyyyyyyyx",
	"xssysyyyssyysysyyyyysyyysyysssyx",
	"xssyyysyyyysyyyyysyyyysyysyyysyx",
	"xsyyssssyssssssyyyyysysysysyyyyx",
	"xsysysssysssyyyyysyysysysyyssyyx",
	"xsssyssyyyyssysyysyyyyysyyyssyyx",
	"xsssyyysysyyyyyyyyyysysysyykkykx",
	"xsyyysyssssysysyysyyyyyysyysssyx",
	"xssysyysssyysysyyyyysysysskkykkx",
	"xssysyssyyysyysssssssssssssssskx",
	"xssssyssyssyyyyysyyyysyysyyksykx",
	"xssyyyyyyssssyyyyysyyyyyyyskykkx",
	"xttttttttttttttttttttttttttttttx"
    };
    Mapa oMapa_05;
    oMapa_05.getPtrMapa(mapa_05);
    arrayMapas[5] = oMapa_05;
}

void Juego::crearArrayJugadores()
{
    if (cantJugadores==1)
    {
        Punto oPunto1(45,45);
        Jugador oPlayer1(oPunto1);
        arrayJugadores[0] = oPlayer1;
    }
    else if (cantJugadores==2)
    {
        Punto oPunto1(45,45);
        Jugador oPlayer1(oPunto1);
        arrayJugadores[0] = oPlayer1;

        Punto oPunto2((MCOL-3)*45,(MFIL-3)*45);
        Jugador oPlayer2(oPunto2);
        arrayJugadores[1] = oPlayer2;
    }
}

void Juego::collisionBomba(int X,int Y,int nivel,int alcanze) //en juego
{
    int y=Y/45;
    int x=X/45;

    int abajo = 1;
    int derecha = 1;
    int arriba = 1;
    int izquierda = 1;

    for(int j=0 ; j<cantJugadores ; j++)
    {
        if (arrayJugadores[j].getCoordenada()->getX()== x*45 && arrayJugadores[j].getCoordenada()->getY()==(y)*45)
            arrayJugadores[j].addSalud(-1);
    }

    for(int i=1; i<=alcanze ;i++)
    {
        if (abajo==1){//colision abajo
        if(arrayMapas[nivel].getPtrMapa()[y+i][x] != 'x' && arrayMapas[nivel].getPtrMapa()[y+i][x] != 't' && arrayMapas[nivel].getPtrMapa()[y+i][x] != 's'){ //colision con el mapa
                arrayMapas[nivel].getPtrMapa()[y+i][x] = 'k';

            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()== x*45 && arrayJugadores[j].getCoordenada()->getY()==(y+i)*45)
                    arrayJugadores[j].addSalud(-1);
            }

        }
        else
            abajo=0;
        }

        if (derecha==1){//colision derecha
        if(arrayMapas[nivel].getPtrMapa()[y][x+i] != 'x' && arrayMapas[nivel].getPtrMapa()[y][x+i] != 't' && arrayMapas[nivel].getPtrMapa()[y][x+i] != 's'){
                arrayMapas[nivel].getPtrMapa()[y][x+i] = 'k';


            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()==(x+i)*45 && arrayJugadores[j].getCoordenada()->getY()==y*45)
                    arrayJugadores[j].addSalud(-1);
            }

        }
        else
            derecha=0;
        }

        if (arriba==1){//colision arriba
        if(arrayMapas[nivel].getPtrMapa()[y-i][x] != 'x' && arrayMapas[nivel].getPtrMapa()[y-i][x] != 't' && arrayMapas[nivel].getPtrMapa()[y-i][x] != 's' ){
                arrayMapas[nivel].getPtrMapa()[y-i][x] = 'k';

            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()==x*45 && arrayJugadores[j].getCoordenada()->getY()==(y-i)*45)
                    arrayJugadores[j].addSalud(-1);
            }
        }
        else
            arriba=0;
        }

        if (izquierda==1){//colision izquierda
        if(arrayMapas[nivel].getPtrMapa()[y][x-i] != 'x' && arrayMapas[nivel].getPtrMapa()[y][x-i] != 't' && arrayMapas[nivel].getPtrMapa()[y][x-i] != 's'){
                arrayMapas[nivel].getPtrMapa()[y][x-i] = 'k';

            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()==(x-i)*45 && arrayJugadores[j].getCoordenada()->getY()==y*45)
                    arrayJugadores[j].addSalud(-1);
            }

        }
        else
            izquierda=0;
        }
    }
}

void Juego::explosionDeBomba(int tiempo,int n_mapa, int alcanze,int n_jugador) //si la bomba es iniciada, su contador sube, y al llegar al tiempo determinado
{                                                                               //la bomba explota (colision bomba)
     for (int i=0;i<max_bombas;i++)
    {
        if(bombas[n_jugador][i].contador!=0){
        bombas[n_jugador][i].contador++;
            if (bombas[n_jugador][i].contador==tiempo){
                collisionBomba(bombas[n_jugador][i].x,bombas[n_jugador][i].y,n_mapa,alcanze);
                bombas[n_jugador][i].x=0;
                bombas[n_jugador][i].y=0;
                bombas[n_jugador][i].contador=0;
            }
        }
    }

}

void Juego:: powerUpGenerator (int &n_powerUp, int nivel, int &t_generacionDePowerUp)
{
    t_generacionDePowerUp--;

    if (t_generacionDePowerUp<=0)
    {
        int x;
        int y;
        if (PowerUps[n_powerUp].contador==0)
        {
            srand((unsigned) time(0));
            x=(rand() % 30) + 1;
            y = (rand() % 17) + 1;
            if(arrayMapas[nivel].getPtrMapa()[y][x] != 'x' && arrayMapas[nivel].getPtrMapa()[y][x] != 't' && arrayMapas[nivel].getPtrMapa()[y][x] != 's')
            {
                PowerUps[n_powerUp].x=x*45;
                PowerUps[n_powerUp].y=y*45;
                PowerUps[n_powerUp].contador=1;
                if((n_powerUp+1) == max_powerUps)
                {
                n_powerUp=0;
                }
                else{
                n_powerUp++;}
                t_generacionDePowerUp=(rand() % 100) + 1;
            }
        }
    }
}

void Juego::ejecucionPowerUps()
{
    for(int i=0;i<=max_powerUps;i++){
        PowerUps[i].checkStatus(arrayJugadores,cantJugadores,tiempo_powerUp,max_bombas);
    }
}

void Juego::checkSaludPlayers(int &status)
{
    for (int i=0 ; i<cantJugadores ; i++)
        if (arrayJugadores[i].checkStatus()==1)
            gameover(status,i);
            //status = 1;
}

void Juego::nextLevel(int &n_mapa, Key &theKey,  Door &theDoor)
{
    crearArrayJugadores();

    Bomba newBombas[2][max_bombas];
    for (int j=0 ; j<cantJugadores;j++){
        for (int i =0 ; i<max_bombas ; i++)
            bombas[j][i] = newBombas[j][i];
    }

    PowerUp newPowerUps[max_powerUps];

    for (int i =0 ; i<max_powerUps ; i++)
        PowerUps[i] = newPowerUps[i];


    if (n_mapa+1 > cantMapas-1)
        n_mapa=0;
    else
        n_mapa++;


    theKey.setKey(n_mapa, arrayMapas);

    theDoor.setStatus(0);
    SAMPLE *sound = load_wav("recursos/pasar_nivel.wav");
    play_sample(sound,250,150,1000,0);

}

void Juego::ejecutar() // inicia la ejecucion del juego
{
    //int max_bombas=5;
    int n_bomba=0;
    int n_bomba2=0;

    //int max_powerUps=3;
    int n_powerUp=0;
    int t_generacionDePowerUp = 100;

    int n_mapa = 0;

    Key theKey(n_mapa,arrayMapas);

    Door theDoor(1260,720);

    int status=0;

    while(status!=1)
    {
        arrayJugadores[0].movimiento(n_mapa,arrayMapas,bombas,max_bombas,n_bomba,1);

        if (cantJugadores==2)
        {
            arrayJugadores[1].movimiento(n_mapa,arrayMapas,bombas,max_bombas,n_bomba2,2);
            explosionDeBomba(30,n_mapa,arrayJugadores[1].getAlcanzeDeBomba(),1);
        }


        if (key[KEY_ESC]) {
            status = pause();
            cout<<"status: "<<status<<endl;
            if (status==1)
                break;
        }

        checkSaludPlayers(status);
        powerUpGenerator(n_powerUp,n_mapa,t_generacionDePowerUp);

        ejecucionPowerUps();
        explosionDeBomba(30,n_mapa,arrayJugadores[0].getAlcanzeDeBomba(),0); //el stack de cada jugador (fila del array)


        clear(buffer);

        dibujarMapa(n_mapa,0);     // el argumento '0'representa el nivel que se va a dibujar

        if (cantJugadores==1)
        {
            if (theDoor.checkStatus(arrayJugadores,cantJugadores)==1) {
                nextLevel(n_mapa,theKey,theDoor);
                cout<<"NextMAP"<<endl;
            }
            theKey.checkStatus(arrayJugadores,cantJugadores);
            theKey.printKey(buffer);
            theDoor.printDoor(buffer,arrayJugadores);
        }


        printBomba(bomba_bmp,buffer,bombas,n_bomba-1,max_bombas);

        printPowerUp(buffer,PowerUps, max_powerUps);

        dibujarJugador(0);

        dibujarMapa(n_mapa, 1);     // el argumento '0'representa el nivel que se va a dibujar

        imprimirStadisticas();

        mostrarPantalla();

        rest(20);           // tiempo de espera en milisegundos para cada refresh
    }
}

void Juego::imprimirStadisticas()
{

    int op=0;
    masked_blit(estadisticas,buffer2,0, 0, 0, 0, 76, MFIL*45);

        BITMAP *number1 = load_bitmap("recursos/number1.bmp",NULL);
        BITMAP *number2 = load_bitmap("recursos/number2.bmp",NULL);
        BITMAP *number3 = load_bitmap("recursos/number3.bmp",NULL);
        BITMAP *number4 = load_bitmap("recursos/number4.bmp",NULL);
        BITMAP *number5 = load_bitmap("recursos/number5.bmp",NULL);
        BITMAP *zapatillas = load_bitmap("recursos/zapatillas.bmp",NULL);
        BITMAP *key = load_bitmap("recursos/key.bmp",NULL);
        //vida
        if(arrayJugadores[op].getSalud()==1)
            masked_blit(number1,buffer2,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==2)
            masked_blit(number2,buffer2,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==3)
            masked_blit(number3,buffer2,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==4)
            masked_blit(number4,buffer2,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==5)
            masked_blit(number5,buffer2,0, 0,  9, 150, 60, 60);

        //n_bomba
        if(arrayJugadores[op].getCantBombas()==1)
            masked_blit(number1,buffer2,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==2)
            masked_blit(number2,buffer2,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==3)
            masked_blit(number3,buffer2,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==4)
            masked_blit(number4,buffer2,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==5)
            masked_blit(number5,buffer2,0, 0,  9, 300, 60, 60);

        //alcanze
        if(arrayJugadores[op].getAlcanzeDeBomba()==1)
            masked_blit(number1,buffer2,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==2)
            masked_blit(number2,buffer2,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==3)
            masked_blit(number3,buffer2,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==4)
            masked_blit(number4,buffer2,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==5)
            masked_blit(number5,buffer2,0, 0,  9, 450, 60, 60);

        //llave
        if(arrayJugadores[op].getKey()==1)
            masked_blit(key,buffer2,0, 0,  15, 550, 45, 45);

         //velocidad
        if(arrayJugadores[op].getVelocidad()!=3)
            masked_blit(zapatillas,buffer2,0, 0,  9, 650, 60, 60);

        if(cantJugadores==2)
        {
            masked_blit(estadisticas,buffer3,0, 0, 0, 0, 76, MFIL*45);
            op=1;
            //vida
        if(arrayJugadores[op].getSalud()==1)
            masked_blit(number1,buffer3,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==2)
            masked_blit(number2,buffer3,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==3)
            masked_blit(number3,buffer3,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==4)
            masked_blit(number4,buffer3,0, 0,  9, 150, 60, 60);
        else if (arrayJugadores[op].getSalud()==5)
            masked_blit(number5,buffer3,0, 0,  9, 150, 60, 60);

        //n_bomba
        if(arrayJugadores[op].getCantBombas()==1)
            masked_blit(number1,buffer3,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==2)
            masked_blit(number2,buffer3,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==3)
            masked_blit(number3,buffer3,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==4)
            masked_blit(number4,buffer3,0, 0,  9, 300, 60, 60);
        if(arrayJugadores[op].getCantBombas()==5)
            masked_blit(number5,buffer3,0, 0,  9, 300, 60, 60);

        //alcanze
        if(arrayJugadores[op].getAlcanzeDeBomba()==1)
            masked_blit(number1,buffer3,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==2)
            masked_blit(number2,buffer3,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==3)
            masked_blit(number3,buffer3,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==4)
            masked_blit(number4,buffer3,0, 0,  9, 450, 60, 60);
        if(arrayJugadores[op].getAlcanzeDeBomba()==5)
            masked_blit(number5,buffer3,0, 0,  9, 450, 60, 60);

        //llave
        if(arrayJugadores[op].getKey()==1)
            masked_blit(key,buffer3,0, 0,  9, 550, 45, 45);

         //velocidad
        if(arrayJugadores[op].getVelocidad()!=3)
            masked_blit(zapatillas,buffer3,0, 0,  9, 650, 60, 60);
        }

}

void Juego::dibujarMapa(int nivel,int op)
{
    int x, y;
    for(x = 0; x < MFIL; x++) {                              // itera por las filas del mapa seleccionado
        for(y = 0; y < MCOL; y++) {                           // itera por cada elemento de la fila del mapa seleccionado
            if (op==0){
                if(arrayMapas[nivel].getPtrMapa()[x][y] == 'x') // si encuentra una 'x' va a dibujar un muro encima
                    draw_sprite(buffer, muro, y*45, x*45-31);

                if(arrayMapas[nivel].getPtrMapa()[x][y] == 'y')
                    draw_sprite(buffer, muro_destructible, y*45, x*45);
                //marco del mapa
                if(arrayMapas[nivel].getPtrMapa()[x][y] == 's') // si encuentra una 's' va a dibujar un muro encima
                    draw_sprite(buffer, muro2, y*45, x*45);
                }

            if (op==1){
                //marco del mapa
                if(arrayMapas[nivel].getPtrMapa()[x][y] == 't')
                    draw_sprite(buffer, muro, y*45, x*45-31);

            }

        }
    }
}

void Juego::dibujarJugador(int pj)
{
    blit(player_bmp, player, dir*45, 0, 0, 0, 45, 90);
    for(int i=0; i<cantJugadores; i++)
    {
        masked_blit(player,buffer,0, 0, arrayJugadores[i].getCoordenada()->getX(), arrayJugadores[i].getCoordenada()->getY()-45, 45, 90);
    }
}

void Juego::mostrarPantalla()
{
    blit(buffer2, screen, 0, 0, 0, 0, 76, MFIL*45);
    if(cantJugadores==1)
        blit(buffer, screen, 0, 0, 76, 0, (MCOL-1)*45, MFIL*45);
    else if(cantJugadores==2){
        blit(buffer, buffer4, 0, 0, -38, 0, (MCOL-1)*45, MFIL*45);
        blit(buffer4, screen, 0, 0, 76, 0, (MCOL-1)*45, MFIL*45);
        blit(buffer3, screen, 0, 0, 1440, 0, 76, MFIL*45);
    }
}

////pause///
int Juego::pause()
{
    install_mouse();
    BITMAP *buffer = create_bitmap(566,319);
    BITMAP *idle = load_bitmap("recursos/pause_idle.bmp",NULL);
    BITMAP *continuar= load_bitmap("recursos/pause_continuar.bmp",NULL);
    BITMAP *salir = load_bitmap("recursos/pause_salir.bmp",NULL);
    BITMAP *cursor = load_bitmap("recursos/cursor.bmp",NULL);

    while(!key[KEY_ESC]){
        if(mouse_x>42&&mouse_x<275&&mouse_y>220&&mouse_y<284){
            blit(continuar,buffer,0,0,0,0,566,319);
            if (mouse_b & 1){
            return 0;
            }
        }
        else if(mouse_x>290&&mouse_x<520&&mouse_y>220&&mouse_y<284){
            blit(salir,buffer,0,0,0,0,566,319);
            if (mouse_b & 1){
            return 1;
            }
        }
        else{
            blit(idle,buffer,0,0,0,0,566,319);

        }
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,50,75);
        blit(buffer, screen, 0,0, 500, 210, 566, 319);
    }

}

////gameover///
void Juego::gameover(int &status,int perdedor)
{
    install_mouse();
    BITMAP *buffer = create_bitmap(566,319);
    BITMAP *go = load_bitmap("recursos/gameover/go.bmp",NULL);
    BITMAP *salir= load_bitmap("recursos/gameover/goSalir.bmp",NULL);
    BITMAP *player1 = load_bitmap("recursos/gameover/player1.bmp",NULL);
    BITMAP *player2 = load_bitmap("recursos/gameover/player2.bmp",NULL);
    BITMAP *cursor = load_bitmap("recursos/cursor.bmp",NULL);

    while(true)
    {
    if(mouse_x>169&&mouse_x<402&&mouse_y>224&&mouse_y<284){
        blit(salir,buffer,0,0,0,0,566,319);
        if (mouse_b & 1){
        status=1;
        break;
        }
    }
    else{
        blit(go,buffer,0,0,0,0,566,319);
    }

    if(cantJugadores==2 && perdedor==0)
        blit(player2,buffer,0,0,0,0,566,319);


    else if (cantJugadores==2 && perdedor==1)
        blit(player1,buffer,0,0,0,0,566,319);


    masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,50,75);
    blit(buffer, screen, 0,0, 500, 210, 566, 319);
    }


}

/////menu///
void menu()
{
    //iniciar();
    install_mouse();
    BITMAP *buffer = create_bitmap(1516,855);
    BITMAP *idle = load_bitmap("recursos/idLe.bmp",NULL);
    BITMAP *aventura = load_bitmap("recursos/aventura.bmp",NULL);
    BITMAP *multiplayer = load_bitmap("recursos/multiplayer.bmp",NULL);
    BITMAP *creditos = load_bitmap("recursos/creditos.bmp",NULL);
    BITMAP *salir = load_bitmap("recursos/salir.bmp",NULL);
    BITMAP *cursor = load_bitmap("recursos/cursor.bmp",NULL);

    ////imagenes en creditos///
    BITMAP *creditos1 = load_bitmap("recursos/creditos1.bmp",NULL);
    BITMAP *creditos2 = load_bitmap("recursos/creditos2.bmp",NULL);
    BITMAP *creditos3 = load_bitmap("recursos/creditos3.bmp",NULL);

    ////musica////
    MIDI *intro = load_midi("recursos/gameintro.mid");
    SAMPLE *soundselect = load_wav("recursos/sound_select.wav");

    //thread musica
    std::thread th1(play_midi, intro, 1);

    blit(idle,buffer,0,0,0,0,1516,855);

    while(true)
    {

        if(mouse_x>136&&mouse_x<586&&mouse_y>404&&mouse_y<528)
        {
            blit(aventura,buffer,0,0,0,0,1516,855);
            if (mouse_b & 1)
            {
                Juego *oJuego = new Juego(1);
                oJuego->ejecutar();
                delete oJuego;
                cout<<"Salio del juego"<<endl;
                // break;
            }
        }
        else if (mouse_x>330&&mouse_x<813&&mouse_y>556&&mouse_y<683)
        {
            blit(multiplayer,buffer,0,0,0,0,1516,855);
            if (mouse_b & 1)
            {
                Juego *oJuego = new Juego(2);
                oJuego->ejecutar();
                delete oJuego;
                cout<<"Salio del juego"<<endl;
                // break;
            }
        }
        else if (mouse_x>442&&mouse_x<805&&mouse_y>732&&mouse_y<829)
        {
            blit(creditos,buffer,0,0,0,0,1516,855);
            if (mouse_b & 1)
            {
                int i=0;
                while(!key[KEY_ESC])
                {

                    if(i>2000)
                    {
                        blit(creditos1,buffer,0,0,0,0,566,319);
                    }
                    else if(i>1000)
                    {
                        blit(creditos2,buffer,0,0,0,0,566,319);
                    }
                    else
                    {
                        blit(creditos3,buffer,0,0,0,0,566,319);
                    }

                    i++;
                    if(i==3000)
                    {
                        i=0;
                    }

                    blit(buffer, screen, 0,0, 450, 250, 566, 319);
                }
            }
        }
        else if (mouse_x>824&&mouse_x<1100&&mouse_y>736&&mouse_y<823)
        {
            blit(salir,buffer,0,0,0,0,1516,855);
            if (mouse_b & 1)
            {
                break;
            }
        }
        else
        {
            play_sample(soundselect,250,150,1000,0);
            blit(idle,buffer,0,0,0,0,1516,1516);
        }
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,50,75);
        blit(buffer, screen, 0, 0, 0, 0, 1516, 855);

    }
    th1.join();
}

void iniciar()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1280, 720, 0, 0);
        if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }
    set_volume(70, 70);
}

int main ()
{
    iniciar();
    menu();

    return 1;
}
END_OF_MAIN ()
