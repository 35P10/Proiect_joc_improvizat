#include <allegro.h>
#define MFIL 20  // filas del mapa
#define MCOL 33  // columnas del mapa, en realidad seran 20(o sea -1) descontando el caracter final '\0' (//solo puede ser impar?)
#include "Punto.h"
#include "Bomba.h"
#include <iostream>
using namespace std;

// == CLASE MAPA ==============================================================
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


// == CLASE PERSONAJE (Padre) =====================================================

class Personaje
{

};

// == CLASE JUGADOR ==============================================================
class Jugador
{
    Punto *coordenada;
    int salud;
    int velocidad;
    int cantBombas;
    void init();
public:
    Jugador();
    Jugador(Punto);
    Punto *getCoordenada();
    void plantarBomba();
    void imprimirDatos();
    void movimiento(int nivel, Mapa *arrayMapas, Bomba *bombas,int max_bombas,int &n_bomba);
};

void Jugador::init() {
    salud      = 10;
    velocidad  = 1;
    cantBombas = 1;
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

void Jugador::movimiento(int nivel, Mapa *arrayMapas,Bomba *bombas,int max_bombas,int &n_bomba)
{
    int Y=coordenada->getY()/30;
    int X=coordenada->getX()/30;
    int y1=Y-1;
    int y2=Y+1;
    int x1=X-1;
    int x2=X+1;

    if(arrayMapas[nivel].getPtrMapa()[y1][X] == 'k' && key[KEY_W] && y1>-1 && IsThereABomb(bombas,max_bombas,X*30,y1*30)==0)
        coordenada->offset(0,-30);
    if(arrayMapas[nivel].getPtrMapa()[y2][X] == 'k' && key[KEY_S]&& y2<18 && IsThereABomb(bombas,max_bombas,X*30,y2*30)==0)
        coordenada->offset(0,+30);
    if(arrayMapas[nivel].getPtrMapa()[Y][x1] == 'k' && key[KEY_A] && x1>-1 && IsThereABomb(bombas,max_bombas,x1*30,Y*30)==0)
        coordenada->offset(-30,0);
    if(arrayMapas[nivel].getPtrMapa()[Y][x2] == 'k' && key[KEY_D] && x2<32 && IsThereABomb(bombas,max_bombas,x2*30,Y*30)==0)
        coordenada->offset(+30,0);
    if (key[KEY_E]) {
            crearBomba(bombas,coordenada,n_bomba,max_bombas);
            if (n_bomba==5)
                n_bomba=0;
    }


}

// == BOMBA ==============================================================

void collisionBomba(int x,int y,int nivel, Mapa *arrayMapas)
{
    int Y=y/30;
    int X=x/30;
    int y1=Y-1;
    int y2=Y+1;
    int x1=X-1;
    int x2=X+1;
    //cout<<"x"<<X<<endl;
    //cout<<"y"<<Y<<endl;
    if(y1>-1 && X>-1 && arrayMapas[nivel].getPtrMapa()[y1][X] != 'x')
        arrayMapas[nivel].getPtrMapa()[y1][X] = 'k';//\0
    if(y2>-1 && X>-1 && arrayMapas[nivel].getPtrMapa()[y2][X]!= 'x')
        arrayMapas[nivel].getPtrMapa()[y2][X] = 'k';//\0
    if(Y>-1 && x2>-1 && arrayMapas[nivel].getPtrMapa()[Y][x1] != 'x')
        arrayMapas[nivel].getPtrMapa()[Y][x1] = 'k';//\0
    if(Y>-1 && x2>-1 && arrayMapas[nivel].getPtrMapa()[Y][x2] != 'x')
        arrayMapas[nivel].getPtrMapa()[Y][x2] = 'k';//\0

}


void explosionDeBomba(Bomba *bombas,int max_bombas,int tiempo,Mapa *arrayMapas)
{
     for (int i=0;i<max_bombas;i++)
    {
        if(bombas[i].contador!=0){
        bombas[i].contador++;
        if (bombas[i].contador==tiempo){
            collisionBomba(bombas[i].x,bombas[i].y,0,arrayMapas);
            bombas[i].x=0;
            bombas[i].y=0;
            bombas[i].contador=0;
        }
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

    int unit = 30;          // tamaño de cada unidad de imagen en el mapa  (en pixeles)
    int cantMapas;   // cantidad de niveles
    Mapa *arrayMapas;       // array con punteros a cada objeto Mapa (niveles)
    int cantJugadores;
    Jugador *arrayJugadores;
    int dir = 0;
    Bomba bombas[5];
public:
    Juego();
    Mapa* getArrayMapas();
    Jugador* getArrayJugadores();
    void crearArrayMapas();
    void crearArrayJugadores();
    void dibujarMapa(int);
    void dibujarJugador(int);
    void mostrarPantalla();
    bool colision();
    void ejecutar();
    int pause();
};

Juego::Juego()
{
    buffer     = create_bitmap((MCOL-1)*30, MFIL*30); // *30 ->es el tam de la imagen (30px x 30px)
    muro       = load_bitmap("muro.bmp", NULL);
    player     = create_bitmap(30, 30);  // ver si necesita mas pixeles
    player_bmp = load_bitmap("player.bmp", NULL);
    muro_destructible  = load_bitmap("muro_destructuble.bmp", NULL);
    bomba_bmp  = load_bitmap("bomba.bmp", NULL);

    cantMapas  = 2; // serán más
    arrayMapas = new Mapa[cantMapas];
    crearArrayMapas();
    cantJugadores  = 2; //
    arrayJugadores = new Jugador[cantJugadores];
    crearArrayJugadores();
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
    "xyyyyyyyyyyyyxyyyxyyyxyyyyyyyyyx",
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
    };                              // "mapa_00" es el diseño del mapa hecho con chars
    Mapa oMapa_00(mapa_00);         // se crea "oMapa_00" que es un objeto de la clase Mapa, basado en el diseño de "mapa_00"
    arrayMapas[0] = oMapa_00;       // se inserta el objeto "oMapa_00" en la primera posicion del array de mapas del juego

    char mapa_01[MFIL][MCOL] = { // NIVEL 1
    "xxxxxxxxxxxxxxxxxxxx",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "x                  x",
    "xxxxxxxxxxxxxxxxxxxx"
    };
    Mapa oMapa_01(mapa_01);
    arrayMapas[1] = oMapa_01;
}

void Juego::crearArrayJugadores()
{
        Punto oPunto1(30,510);
        Jugador oPlayer1(oPunto1);      //aqui llenar más jugadores
        arrayJugadores[0] = oPlayer1;

        Punto oPunto2((MCOL-3)*30,(MFIL-2)*30);
        Jugador oPlayer2(oPunto2);
        arrayJugadores[1] = oPlayer2;
}



void Juego::ejecutar() // inicia la ejecucion del juego
{
    int max_bombas=5;
    int n_bomba=0;
    int status=0;
    while(status!=1)
    {
        arrayJugadores[0].movimiento(0,arrayMapas,bombas,max_bombas,n_bomba);


        if (key[KEY_ESC]) {
            status = pause();

        }
        explosionDeBomba(bombas,max_bombas,30,arrayMapas);
        clear(buffer);
        dibujarMapa(0);     // el argumento '0'representa el nivel que se va a dibujar
        dibujarJugador(0);
        dibujarJugador(1);
        printBomba(bomba_bmp,buffer,bombas,n_bomba-1,max_bombas);
        mostrarPantalla();
        rest(70);           // tiempo de espera en milisegundos para cada refresh
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
                draw_sprite(buffer, muro, y*30, x*30);
            if(arrayMapas[nivel].getPtrMapa()[x][y] == 'y')
                draw_sprite(buffer, muro_destructible, y*30, x*30);
        }
    }
}

void Juego::dibujarJugador(int pj)
{
    blit(player_bmp, player, dir*33, 0, 0, 0, 33, 33);
    draw_sprite(buffer, player, arrayJugadores[pj].getCoordenada()->getX(), arrayJugadores[pj].getCoordenada()->getY());
}
void Juego::mostrarPantalla()
{
    blit(buffer, screen, 0, 0, 0, 0, (MCOL-1)*30, MFIL*30);
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
    BITMAP *buffer = create_bitmap(960,540);
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

    blit(idle,buffer,0,0,0,0,960,540);
https://www.youtube.com/watch?v=kV5Uq8w9-6Y
    while(true){

    if(mouse_x>140&&mouse_x<340&&mouse_y>282&&mouse_y<311)
    {
      blit(aventura,buffer,0,0,0,0,960,540);
      if (mouse_b & 1)
      {
        Juego oJuego;
        oJuego.ejecutar();
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
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 960, 570, 0, 0);
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
