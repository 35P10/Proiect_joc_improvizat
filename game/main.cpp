#include <allegro.h>
#define MFIL 20  // filas del mapa
#define MCOL 33  // columnas del mapa, en realidad seran 20(o sea -1) descontando el caracter final '\0' (//solo puede ser impar?)
#include "Punto.h"
#include "Mapa.h"
#include "Bomba.h"

#include "Key.h"
#include "PowerUp.h"
#include "Door.h"

//#include "Jugador.h"

#include <ctime>
#include <iostream>
using namespace std;


// == BOMBA ==============================================================

void collisionBomba(int X,int Y,int nivel, Mapa *arrayMapas,Jugador *arrayJugadores, int cantJugadores, int alcanze) //en juego
{
    int y=Y/45;
    int x=X/45;
    //int alcanze=2;

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
        if(arrayMapas[nivel].getPtrMapa()[y+i][x] != 'x'){ //colision con el mapa
            arrayMapas[nivel].getPtrMapa()[y+i][x] = 'k';

            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()== x*45 && arrayJugadores[j].getCoordenada()->getY()==(y+i)*45)
                    arrayJugadores[j].addSalud(-1);
            }

            //cout<<"colision abajo: "<<y+i<<endl;
            //cout<<"x: "<<y+i<<endl;
            //cout<<"y: "<<x<<endl;
            }
        else
            abajo=0;
        }

        if (derecha==1){//colision derecha
        if(arrayMapas[nivel].getPtrMapa()[y][x+i] != 'x'){
            arrayMapas[nivel].getPtrMapa()[y][x+i] = 'k';


            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()==(x+i)*45 && arrayJugadores[j].getCoordenada()->getY()==y*45)
                    arrayJugadores[j].addSalud(-1);
            }


            //cout<<"colision derecha: "<<y+i<<endl;
            //cout<<"x: "<<y<<endl;
            //cout<<"y: "<<x+i<<endl;
            }
        else
            derecha=0;
        }

        if (arriba==1){//colision arriba
        if(arrayMapas[nivel].getPtrMapa()[y-i][x] != 'x'){
            arrayMapas[nivel].getPtrMapa()[y-i][x] = 'k';

            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()==x*45 && arrayJugadores[j].getCoordenada()->getY()==(y-i)*45)
                    arrayJugadores[j].addSalud(-1);
            }

            //cout<<"colision abajo: "<<y+i<<endl;
            //cout<<"x: "<<y+i<<endl;
            //cout<<"y: "<<x<<endl;
            }
        else
            arriba=0;
        }

        if (izquierda==1){//colision izquierda
        if(arrayMapas[nivel].getPtrMapa()[y][x-i] != 'x'){
            arrayMapas[nivel].getPtrMapa()[y][x-i] = 'k';

            for(int j=0 ; j<cantJugadores ; j++)
            {
                if (arrayJugadores[j].getCoordenada()->getX()==(x-i)*45 && arrayJugadores[j].getCoordenada()->getY()==y*45)
                    arrayJugadores[j].addSalud(-1);
            }

            //cout<<"colision derecha: "<<y+i<<endl;
            //cout<<"x: "<<y<<endl;
            //cout<<"y: "<<x+i<<endl;
            }
        else
            izquierda=0;
        }


    }
}


// == CLASE JUEGO =================================================================

class Juego
{
    BITMAP *buffer;         // buffer: se asigna su valor en el main, buffer es el espacio donde se insertan los sprites(imagenes)
    BITMAP *muro;           // muro: tambien se asigna en el main, es la variable que va a contener la imagen con q se hacen los muros
    BITMAP *player;         // player: buffer de personaje
    BITMAP *player_bmp;     // player_bmp: imagen bmp del personaje
    BITMAP *bomba_bmp;
    BITMAP *muro_destructible;

    int unit = 45;          // tamaño de cada unidad de imagen en el mapa  (en pixeles)
    int cantMapas;   // cantidad de niveles
    Mapa *arrayMapas;       // array con punteros a cada objeto Mapa (niveles)
    int cantJugadores;
    Jugador *arrayJugadores;
    int dir = 0;

    Bomba bombas[5];
    int max_bombas = 5;

    PowerUp PowerUps[3];
    int max_powerUps = 3;
    int tiempo_powerUp = 300;


public:
    Juego();
    ~Juego();
    Mapa* getArrayMapas();
    Jugador* getArrayJugadores();


    void crearArrayMapas();
    void crearArrayJugadores();
    void dibujarMapa(int);
    void dibujarJugador(int);
    void mostrarPantalla();
    bool colision();
    void ejecutar();
    void ejecucionPowerUps();
    void nextLevel(int &n_mapa,Key &theKey, Door &theDoor);
    void powerUpGenerator (int &n_powerUp, int nivel, int &t_generacionDePowerUp);
    void explosionDeBomba(int tiempo,int n_mapa,int alcanze);
    int pause();
    void checkSaludPlayers(int &status);

};



Juego::Juego()
{
    buffer     = create_bitmap((MCOL-1)*45, MFIL*45); // *30 ->es el tam de la imagen (30px x 30px)
    muro       = load_bitmap("muro.bmp", NULL);
    player     = create_bitmap(45, 90);  // ver si necesita mas pixeles
    player_bmp = load_bitmap("player.bmp", NULL);
    muro_destructible  = load_bitmap("muro_destructuble.bmp", NULL);
    bomba_bmp  = load_bitmap("bomba.bmp", NULL);

    cantMapas  = 3; // serán más
    arrayMapas = new Mapa[cantMapas];
    crearArrayMapas();
    cantJugadores  = 2; //
    arrayJugadores = new Jugador[cantJugadores];
    crearArrayJugadores();
}

Juego::~Juego()
{

   delete []buffer;         // buffer: se asigna su valor en el main, buffer es el espacio donde se insertan los sprites(imagenes)
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
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //alto =17, ancho=20
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",//xyyyyyyxyyyyyyyxyyyxyxyyyxyyyxyx
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    };                              // "mapa_00" es el diseño del mapa hecho con chars
    Mapa oMapa_00(mapa_00);         // se crea "oMapa_00" que es un objeto de la clase Mapa, basado en el diseño de "mapa_00"
    arrayMapas[0] = oMapa_00;       // se inserta el objeto "oMapa_00" en la primera posicion del array de mapas del juego

    char mapa_01[MFIL][MCOL] = { // NIVEL 1
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //alto =17, ancho=20
    "xkkyyyyyyyyyyxyyyxyyyxyyyyyyyyyx",
    "xyxxyxyxyxyxyxyxyxyxyxyxyxyxyxyx",
    "xyyyyyyxyyyyyxyxyxyxyxyyyyyyyyyx",
    "xyxyxxyxyxxxyxyxyxyxyyyxyxyxyxyx",
    "xyyyyyyxyyyyyxyxyxyxxxyyyyyyyyyx",
    "xyxxyxyxyxyxyxyxyxyxxxyxyxyxyxyx",
    "xyyyyyyxyyyyyxyxyxyxyyyyyyyyyyyx",
    "xyxxyxyxyxxxyxyxyxyxxxxxxxxxxxyx",
    "xyyyyyyxyyyyyxyxyxyxyyyyyyyyyyyx",
    "xyxyxxyxyxyxyxyxyxyxyxxxxxxxxxxx",
    "xyyyyyyxyyyyyxyxyxyxyyyyyyyyyyyx",
    "xyxxyxyxyxxxyxyxyxyxxxxxxxxxxxyx",
    "xyyyyyyxyyyyyxyxyxyxyyyyyyyyyyyx",
    "xyxxyxyxyxyxyxyxyxyxyxxxxxxxxxxx",
    "xyyyyyyxyyyyyxyxyxyxyyyxyyyxyyyx",
    "xkxyxxyxyxxxyxyxyxyxyxyxyxyxyxyx",//
    "xkkyyyyxyyyyyyyxyyyxyxyyyxyyyxyx",//xyyyyyyxyyyyyyyxyyyxyxyyyxyyyxyx
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    };
    Mapa oMapa_01(mapa_01);
    arrayMapas[1] = oMapa_01;

    char mapa_02[MFIL][MCOL] = { // NIVEL 2
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", //alto =17, ancho=20
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkyyyyyyykkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkykkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",
    "xkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx",//xyyyyyyxyyyyyyyxyyyxyxyyyxyyyxyx
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    };
    Mapa oMapa_02(mapa_02);
    arrayMapas[2] = oMapa_02;
}

void Juego::crearArrayJugadores()
{
        Punto oPunto1(45,45);
        Jugador oPlayer1(oPunto1);      //aqui llenar más jugadores
        arrayJugadores[0] = oPlayer1;

        Punto oPunto2((MCOL-3)*45,(MFIL-3)*45);
        Jugador oPlayer2(oPunto2);
        arrayJugadores[1] = oPlayer2;
}


void Juego::explosionDeBomba(int tiempo,int n_mapa, int alcanze) //si la bomba es iniciada, su contador sube, y al llegar al tiempo determinado
{                                                                               //la bomba explota (colision bomba)
     for (int i=0;i<max_bombas;i++)
    {
        if(bombas[i].contador!=0){
        bombas[i].contador++;
        if (bombas[i].contador==tiempo){
            collisionBomba(bombas[i].x,bombas[i].y,n_mapa,arrayMapas,arrayJugadores,cantJugadores,alcanze);
            bombas[i].x=0;
            bombas[i].y=0;
            bombas[i].contador=0;
        }
        }
    }
}


void Juego:: powerUpGenerator (int &n_powerUp, int nivel, int &t_generacionDePowerUp)
{
    t_generacionDePowerUp--;
    //cout<<t_generacionDePowerUp<<endl;
    //cout<<"n_powerUp: "<<n_powerUp<<endl;
    if (t_generacionDePowerUp<=0)
    {
        //cout<<"tiempo De Generacion en 0"<<endl;
        int x;
        int y;
        if (PowerUps[n_powerUp].contador==0){
            srand((unsigned) time(0));
            x=(rand() % 30) + 1;
            y = (rand() % 17) + 1;
            if(arrayMapas[nivel].getPtrMapa()[y][x] != 'x')
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
            status = 1;
}

void Juego::nextLevel(int &n_mapa, Key &theKey,  Door &theDoor)
{


    crearArrayJugadores();

    Bomba newBombas[max_bombas];
    for (int i =0 ; i<max_bombas ; i++)
        bombas[i] = newBombas[i];
    cout<<"powerup"<<endl;
    PowerUp newPowerUps[max_powerUps];
    for (int i =0 ; i<max_powerUps ; i++)
        PowerUps[i] = newPowerUps[i];
    cout<<"mapa"<<endl;
    if (n_mapa+1 > cantMapas-1)
        n_mapa=0;
    else
        n_mapa++;
    cout<<"key"<<endl;
    theKey.setKey(n_mapa, arrayMapas);
    cout<<"door"<<endl;
    theDoor.setStatus(0);

}


void Juego::ejecutar() // inicia la ejecucion del juego
{

    //int max_bombas=5;
    int n_bomba=0;

    //int max_powerUps=3;
    int n_powerUp=0;
    int t_generacionDePowerUp = 100;

    int n_mapa = 0;

    Key theKey(n_mapa,arrayMapas);

    Door theDoor(360,360);

    int status=0;


    while(status!=1)
    {
        arrayJugadores[0].movimiento(n_mapa,arrayMapas,bombas,max_bombas,n_bomba);


        if (key[KEY_ESC]) {
            status = pause();
            cout<<"status: "<<status<<endl;
            if (status==1)
                break;
        }


        if (theDoor.checkStatus(arrayJugadores,cantJugadores)==1) {
            nextLevel(n_mapa,theKey,theDoor);
            cout<<"NextMAP"<<endl;
        }



        checkSaludPlayers(status);
        powerUpGenerator(n_powerUp,n_mapa,t_generacionDePowerUp);
        //contadorPowerUp(PowerUps,max_powerUps,300);
        ejecucionPowerUps();
        explosionDeBomba(30,n_mapa,arrayJugadores[0].getAlcanzeDeBomba()); //el stack de cada jugador (fila del array)

        clear(buffer);

        dibujarMapa(n_mapa);     // el argumento '0'representa el nivel que se va a dibujar

        theKey.checkStatus(arrayJugadores,cantJugadores);
        dibujarJugador(0);
        dibujarJugador(1);
        printBomba(bomba_bmp,buffer,bombas,n_bomba-1,max_bombas);
        printPowerUp(bomba_bmp,buffer,PowerUps, max_powerUps);
        theKey.printKey(buffer);
        theDoor.printDoor(buffer);
        mostrarPantalla();
        rest(20);           // tiempo de espera en milisegundos para cada refresh
    }
}



//x, y ->coordenadas en la matriz del mapa
//buffer-> area donde se va a dibujar(insertar sprites)
//muro->variable que representa la imagen png que se va a insertar para los muros
//x*10, y*10-> pq la imagen del muro es de 10px x 10px




void Juego::dibujarMapa(int nivel)
{
    int x, y;
    for(x = 0; x < MFIL; x++) {                              // itera por las filas del mapa seleccionado
        for(y = 0; y < MCOL; y++) {                           // itera por cada elemento de la fila del mapa seleccionado
            if(arrayMapas[nivel].getPtrMapa()[x][y] == 'x') // si encuentra una 'x' va a dibujar un muro encima
                draw_sprite(buffer, muro, y*45, x*45);
            if(arrayMapas[nivel].getPtrMapa()[x][y] == 'y')
                draw_sprite(buffer, muro_destructible, y*45, x*45);
        }
    }
}

void Juego::dibujarJugador(int pj)
{
    blit(player_bmp, player, dir*45, 0, 0, 0, 45, 90);
    draw_sprite(buffer, player, arrayJugadores[pj].getCoordenada()->getX(), arrayJugadores[pj].getCoordenada()->getY()-45);
}
void Juego::mostrarPantalla()
{
    blit(buffer, screen, 0, 0, 0, 0, (MCOL-1)*45, MFIL*45);
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
        blit(buffer, screen, 0,0, 180, 130, 566, 319);
    }

}

/////menu///

void menu()
{
    //iniciar();
    install_mouse();
    BITMAP *buffer = create_bitmap(960,550);
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
    play_midi(intro,1);

    blit(idle,buffer,0,0,0,0,960,550);

    while(true){

    if(mouse_x>140&&mouse_x<340&&mouse_y>282&&mouse_y<311)
    {
      blit(aventura,buffer,0,0,0,0,960,570);
      if (mouse_b & 1)
      {
        Juego *oJuego = new Juego;
        oJuego->ejecutar();
        delete oJuego;
        cout<<"Salio del juego"<<endl;
       // break;
      }
    }
    else if (mouse_x>236&&mouse_x<490&&mouse_y>373&&mouse_y<408)
    {
      blit(multiplayer,buffer,0,0,0,0,960,540);
    }
    else if (mouse_x>277&&mouse_x<512&&mouse_y>460&&mouse_y<521)
    {
      blit(creditos,buffer,0,0,0,0,960,540);
      if (mouse_b & 1)
      {
          int i=0;
          while(!key[KEY_ESC])
          {

                if(i>300){
                    blit(creditos1,buffer,0,0,0,0,566,319);
                }
                else if(i>200){
                    blit(creditos2,buffer,0,0,0,0,566,319);
                }
                else
                {
                    blit(creditos3,buffer,0,0,0,0,566,319);
                }
                i++;

                if(i==400){i=0;}

                blit(buffer, screen, 0,0, 180, 130, 566, 319);
            }
      }
    }
    else if (mouse_x>520&&mouse_x<693&&mouse_y>463&&mouse_y<521)
    {
      blit(salir,buffer,0,0,0,0,960,540);
      if (mouse_b & 1)
      {
        break;
      }
    }
    else
    {
        play_sample(soundselect,250,150,1000,0);
        blit(idle,buffer,0,0,0,0,960,540);
    }
    masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,50,75);
    blit(buffer, screen, 0, 0, 0, 0, 960, 540);

    }


}

void iniciar()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1440, 855, 0, 0);
        if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }
    set_volume(70, 70);
}

int main ()
{
    //inicioAllegro();
    iniciar();
    menu();
    //Juego oJuego;
    //oJuego.ejecutar();
}
END_OF_MAIN ()
