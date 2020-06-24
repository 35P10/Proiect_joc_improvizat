#include <allegro.h>
#define MFIL 19  // filas del mapa
#define MCOL 33  // columnas del mapa, en realidad seran 20(o sea -1) descontando el caracter final '\0' (//solo puede ser impar?)

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

// == CLASE BOMBA ==============================================================

class Bomba
{
    int potencia;
    float tiempo;
};

// == CLASE PUNTO ==============================================================
class Punto
{
    int x, y;
public:
    Punto();
    Punto(int, int);
    Punto(Punto &);
    int getX();
    int getY();
    void offset(int, int);
    void imprimirPunto();
};

Punto::Punto() {
    x = 0; y = 0;
}
Punto::Punto(int x, int y) {
    this->x = x; this->y = y;
}
Punto::Punto(Punto &oPunto) {
    x = oPunto.x; y = oPunto.y;
}
int Punto::getX() {
    return x;
}
int Punto::getY() {
    return y;
}
void Punto::offset(int offsetX, int offsetY)
{
    x += offsetX; y += offsetY;
}
void Punto::imprimirPunto() {
    cout << "x = " << x << ", y = " << y << endl;
}

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
void Jugador::plantarBomba() {

}
void Jugador::imprimirDatos() {
    coordenada->imprimirPunto();
    cout << salud << endl;
    cout << velocidad << endl;
    cout << cantBombas << endl;
}

// == CLASE ENEMIGO ==============================================================
class Enemigo
{

};

// == CLASE JUEGO =================================================================

class Juego
{
    BITMAP *buffer;         // buffer: se asigna su valor en el main, buffer es el espacio donde se insertan los sprites(imagenes)
    BITMAP *muro;           // muro: tambien se asigna en el main, es la variable que va a contener la imagen con q se hacen los muros
    BITMAP *player;         // player: buffer de personaje
    BITMAP *player_bmp;     // player_bmp: imagen bmp del personaje
    BITMAP *bomba;
    BITMAP *bomba_bmp;
    int unit = 30;          // tamaño de cada unidad de imagen en el mapa  (en pixeles)
    int cantMapas;   // cantidad de niveles
    Mapa *arrayMapas;       // array con punteros a cada objeto Mapa (niveles)
    int cantJugadores;
    Jugador *arrayJugadores;
    int dir = 0;
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
    bomba      = create_bitmap(30,30);
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
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
    "x                              x",
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
        Punto oPunto1(30,30);
        Jugador oPlayer1(oPunto1);      //aqui llenar más jugadores
        arrayJugadores[0] = oPlayer1;

        Punto oPunto2((MCOL-3)*30,(MFIL-2)*30);
        Jugador oPlayer2(oPunto2);
        arrayJugadores[1] = oPlayer2;
}

bool Juego::colision()
{
    //if ()
}

void Juego::ejecutar() // inicia la ejecucion del juego
{
    int status=0;
    while(status!=1)
    {
        if (key[KEY_LEFT] ) {
            dir = 0;
            arrayJugadores[0].getCoordenada()->offset(-30,0); // disminuir en X
        }
        else if (key[KEY_RIGHT]){
            dir = 1;
            arrayJugadores[0].getCoordenada()->offset(30,0); // aumentar en X
        }
        else if (key[KEY_UP]) {
            dir = 2;
            arrayJugadores[0].getCoordenada()->offset(0,-30); // disminuir 1 en Y
        }
        else if (key[KEY_DOWN]) {
            dir = 3;
            arrayJugadores[0].getCoordenada()->offset(0,30); // aumentar 1 en Y
        }
        else if (key[KEY_E]) {
            arrayJugadores[0].plantarBomba();

        }
        else if (key[KEY_ESC]) {
            status = pause();

        }

        clear(buffer);
        dibujarMapa(0);     // el argumento '0'representa el nivel que se va a dibujar
        dibujarJugador(0);
        dibujarJugador(1);
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


    blit(idle,buffer,0,0,0,0,960,540);

    while(true){

    if (mouse_x>140&&mouse_x<340&&mouse_y>282&&mouse_y<311)
    {
      blit(aventura,buffer,0,0,0,0,960,540);
      if (mouse_b & 1)
      {
        Juego oJuego;
        oJuego.ejecutar();
        break;
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
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 960, 540, 0, 0);
        if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }
    set_volume(70, 70);
}
/*

inline void inicioAllegro()
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, (MCOL-1)*30, MFIL*30, 0, 0);
}
*/
// == MAIN ==============================================================

int main ()
{
    //inicioAllegro();
    iniciar();
    menu();
    //Juego oJuego;
    //oJuego.ejecutar();
}
END_OF_MAIN ()
