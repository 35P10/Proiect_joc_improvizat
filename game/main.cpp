#include <allegro.h>
#define MFIL 19  // filas del mapa
#define MCOL 36  // columnas del mapa, en realidad seran -1 descontando el caracter final '\0'. algunas medidas posibles: 24, 28, 36
#define UNIT 37  // tamanio de cada unidad en el mapa
#define P_ALTO 70	// Altura del pj
#define P_ANCHO 35	// Ancho del pj
int BUFFER_ALTO = MFIL*UNIT;  // Alto en pixeles de la pantalla
int BUFFER_ANCHO = ((MCOL-1)*UNIT)+1;  // Ancho en pixeles de la pantalla

#include <iostream>
using namespace std;

// == CLASE MAPA =================================================================================================================
class Mapa
{
    char**ptrMapa;
    friend class Juego;
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
    friend class Juego;
public:
    Punto();
    Punto(int, int);
    Punto(Punto &);
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
    int direccion;
    int salud;
    int velocidad;
    int cantBombas;
    bool enMovimiento;
    void init();
    friend class Juego;
public:
    Jugador();
    Jugador(Punto);
    void plantarBomba();
    void imprimirDatos();
};

void Jugador::init() {
	direccion  = 3;
    salud      = 10;
    velocidad  = 3;
    cantBombas = 1;
    enMovimiento = false;
    coordenada = new Punto(0,0);
}
Jugador::Jugador() {
    init();
}
Jugador::Jugador(Punto coord) {
    init();
    *coordenada = coord;				// usa ctor. copia de Punto
}
void Jugador::plantarBomba() {

}
void Jugador::imprimirDatos() {
    coordenada->imprimirPunto();
    cout << salud << endl;
    cout << velocidad << endl;
    cout << cantBombas << endl;
    cout << direccion << endl;
}

// == CLASE ENEMIGO ==============================================================
class Enemigo
{

};

// == CLASE JUEGO =================================================================
class Juego
{
    BITMAP *buffer_juego;   // buffer: se asigna su valor en el main, buffer es el espacio donde se insertan los sprites(imagenes)
    BITMAP *muro;           // muro: tambien se asigna en el main, es la variable que va a contener la imagen con q se hacen los muros
    BITMAP *player;         // player: buffer de personaje
    BITMAP *player_bmp;     // player_bmp: imagen bmp del personaje
    BITMAP *bomba;
    BITMAP *bomba_bmp;
    int cantMapas;   		// cantidad de niveles
    Mapa *arrayMapas;       // array con punteros a cada objeto Mapa (niveles)
    int cantJugadores;
    Jugador *arrayJugadores;
    int contReloj;
public:
    Juego();
    Mapa* getArrayMapas();
    Jugador* getArrayJugadores();
    void moverJugador(int, int, int);	// (pj, movimiento en x, movimiento en y)
    void crearArrayMapas();
    void crearArrayJugadores();
    void dibujarMapa(int);
    void dibujarJugador(int);
    void mostrarPantalla();
	bool hayColision(int, int);
    void ejecutar(int);					// # del mapa
    int pause();
    void menu();
};

Juego::Juego()
{
    buffer_juego = create_bitmap((MCOL-1)*UNIT, MFIL*UNIT); // *UNIT ->es el tam de la imagen (45px x 45px)
    muro       = load_bitmap("muro_37x37.bmp", NULL);
    player     = create_bitmap(P_ANCHO, P_ALTO);  // ver si necesita mas pixeles
    player_bmp = load_bitmap("pj1_tileset_70x35_2.bmp", NULL);
    bomba      = create_bitmap(UNIT,UNIT);
    bomba_bmp  = load_bitmap("bomba.bmp", NULL);
    contReloj  = 0;

    cantMapas  = 1; // serán más
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
void Juego::moverJugador(int pj, int movX, int movY){	// recibe parametro del jugador y direccion de movimiento
	cout<<"Esta moviendo"<<endl;
	arrayJugadores[0].enMovimiento = true;
	arrayJugadores[pj].coordenada->offset(arrayJugadores[pj].velocidad*movX, arrayJugadores[pj].velocidad*movY);
}
void Juego::crearArrayMapas()
{
    char mapa_00[MFIL][MCOL] = { // NIVEL 0

	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
   	"x         x                       x",
   	"x   xxx   x x x                   x",
   	"x   x x   x                       x",
   	"x   xxx   x x x                   x",
   	"x         x                       x",
   	"xxxx   xxxxxxxxxxxxxxxxxx         x",
   	"x                                 x",
   	"x  x x x xxxx  xx xx              x",
   	"x              xx xx              x",
   	"x  x x x xxxx                     x",
   	"x                                 x",
   	"x                                 x",
   	"x                                 x",
   	"x                                 x",
   	"x                                 x",
   	"x                                 x",
   	"x                                 x",
   	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
   	};
   	                              // "mapa_00" es el diseño del mapa hecho con chars
    Mapa oMapa_00(mapa_00);         // se crea "oMapa_00" que es un objeto de la clase Mapa, basado en el diseño de "mapa_00"
    arrayMapas[0] = oMapa_00;       // se inserta el objeto "oMapa_00" en la primera posicion del array de mapas del juego
/*
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
    */
}

void Juego::crearArrayJugadores()
{
        Punto oPunto1(UNIT, UNIT);
        Jugador oPlayer1(oPunto1);      //aqui llenar más jugadores
        arrayJugadores[0] = oPlayer1;
/*
        Punto oPunto2((MCOL-3)*30,(MFIL-2)*30);
        Jugador oPlayer2(oPunto2);
        arrayJugadores[1] = oPlayer2;
*/
}

bool Juego::hayColision(int pj, int mapa)
{
    int dir = arrayJugadores[pj].direccion;
    int vel = arrayJugadores[pj].velocidad;
    int posX_pj = arrayJugadores[pj].coordenada->x;
    int posY_pj = arrayJugadores[pj].coordenada->y;

    // rango de colision del pj
    int posX_izq = posX_pj+10; //12
    int posX_der = posX_pj+P_ANCHO-10; //12
    int posY_sup = posY_pj+P_ANCHO+20; // 24
    int posY_inf = posY_pj+(2*P_ANCHO);

    //aqui se simula el siguiente paso del pj para verificar si habria colision
    switch(dir){
    	case 0: 										//si se moviera hacia la izquierda
    		posX_izq-=vel; 								// vel (velocidad): representa un unidad de movimiento del pj / dar un paso
    		for(int i=posY_sup; i<=posY_inf; i++){		//va a recorrer todo el borde izquiero del boxcolider buscando colision
    			if(arrayMapas[mapa].ptrMapa[i/UNIT][posX_izq/UNIT]=='x')
    				return true;
			}
			return false;
		case 1:											//si se moviera hacia la derecha
			posX_der+=vel;
    		for(int i=posY_sup; i<=posY_inf; i++){
    			if(arrayMapas[mapa].ptrMapa[i/UNIT][posX_der/UNIT]=='x')
    				return true;
			}
			return false;
		case 2:											//si se moviera hacia la derecha
			posY_sup-=vel;
    		for(int i=posX_izq; i<=posX_der; i++){
    			if(arrayMapas[mapa].ptrMapa[posY_sup/UNIT][i/UNIT]=='x')
    				return true;
			}
			return false;
		case 3:											//si se moviera hacia la derecha
			posY_inf+=vel;
    		for(int i=posX_izq; i<=posX_der; i++){
    			if(arrayMapas[mapa].ptrMapa[posY_inf/UNIT][i/UNIT]=='x')
    				return true;
			}
			return false;
	}
}

void Juego::ejecutar(int mapa) // inicia la ejecucion del juego
{
    int status=0;
    while(status!=1)
    {
        if (key[KEY_LEFT] ) {
            arrayJugadores[0].direccion=0;
            if(hayColision(0, mapa)==false) {
            	moverJugador(0, -1, 0);
            }

        }
        else if (key[KEY_RIGHT]){
            arrayJugadores[0].direccion=1;
            if(hayColision(0, mapa)==false) {
            	moverJugador(0, 1, 0);
            }
        }
        else if (key[KEY_UP]) {
            arrayJugadores[0].direccion=2;
            if(hayColision(0, mapa)==false) {
            	moverJugador(0, 0, -1);
            }
        }
        else if (key[KEY_DOWN]) {
            arrayJugadores[0].direccion=3;
            if(hayColision(0, mapa)==false) {
            	moverJugador(0, 0, 1);
            }
        }
        else if (key[KEY_E]) {
            arrayJugadores[0].plantarBomba();

        }
        else if (key[KEY_ESC]) {
            status = pause();
        }
        arrayJugadores[0].coordenada->imprimirPunto();
        clear(buffer_juego);
        dibujarMapa(0);     // el argumento '0'representa el nivel que se va a dibujar
        dibujarJugador(0);
        //dibujarJugador(1);
        mostrarPantalla();

        rest(10);           // tiempo de espera en milisegundos para cada refresh
        contReloj++;
        if (contReloj > 40)
            contReloj = 0;
        cout << "reloj = " << contReloj<< endl;

    }
}

//x, y ->coordenadas en la matriz del mapa
//buffer-> area donde se va a dibujar(insertar sprites)
//muro->variable que representa la imagen png que se va a insertar para los muros
//x*10, y*10-> pq la imagen del muro es de 10px x 10px
void Juego::dibujarMapa(int nivel)
{
    int x, y;
    for(x = 0; x < MFIL; x++) {                             // itera por las filas del mapa seleccionado
        for(y = 0; y < MCOL; y++) {                         // itera por cada elemento de la fila del mapa seleccionado
            if(arrayMapas[nivel].getPtrMapa()[x][y] == 'x') // si encuentra una 'x' va a dibujar un muro encima
                draw_sprite(buffer_juego, muro, y*UNIT, x*UNIT);
        }
    }
}
void Juego::dibujarJugador(int pj)
{
	int dir = arrayJugadores[pj].direccion;
	int posX = arrayJugadores[pj].coordenada->x;
	int posY = arrayJugadores[pj].coordenada->y;
    bool enMovimiento = arrayJugadores[pj].enMovimiento;

    if (enMovimiento) {
        int spritePj = contReloj/10;
        blit(player_bmp, player, spritePj*P_ANCHO, dir*P_ALTO, 0, 0, P_ANCHO, P_ALTO);
        arrayJugadores[pj].enMovimiento = false;
    }
    else
        blit(player_bmp, player, 0, dir*P_ALTO, 0, 0, P_ANCHO, P_ALTO);

    draw_sprite(buffer_juego, player, posX, posY);
}
void Juego::mostrarPantalla()
{
    blit(buffer_juego, screen, 0, 0, 0, 0, (MCOL-1)*UNIT, MFIL*UNIT);
}
////pause///

int Juego::pause()
{
    install_mouse();
    BITMAP *buffer_pause = create_bitmap(566,319);
    BITMAP *idle = load_bitmap("recursos/pause_idle.bmp",NULL);
    BITMAP *continuar= load_bitmap("recursos/pause_continuar.bmp",NULL);
    BITMAP *salir = load_bitmap("recursos/pause_salir.bmp",NULL);
    BITMAP *cursor = load_bitmap("recursos/cursor.bmp",NULL);

    while(!key[KEY_ESC]){
        if(mouse_x>42&&mouse_x<275&&mouse_y>220&&mouse_y<284){
            blit(continuar,buffer_pause,0,0,0,0,566,319);
            if (mouse_b & 1){
            return 0;
            }
        }
        else if(mouse_x>290&&mouse_x<520&&mouse_y>220&&mouse_y<284){
            blit(salir,buffer_pause,0,0,0,0,566,319);
            if (mouse_b & 1){
            return 1;
            }
        }
        else{
            blit(idle,buffer_pause,0,0,0,0,566,319);
        }
        masked_blit(cursor,buffer_pause,0,0,mouse_x,mouse_y,50,75);
        blit(buffer_pause, screen, 0,0, 180, 130, 566, 319);
    }

}

/////menu///

void Juego::menu()
{
    //iniciar();
    BITMAP *buffer_menu = create_bitmap(BUFFER_ANCHO, BUFFER_ALTO);
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

    blit(idle,buffer_menu,0,0,0,0,960,540);

    while(true){
	    if (mouse_x>140&&mouse_x<340&&mouse_y>282&&mouse_y<311)
	    {
	    	play_sample(soundselect,250,150,1000,0);
	      	blit(aventura,buffer_menu,0,0,0,0,960,540);
	      	if (mouse_b & 1)
	      	{
	        	ejecutar(0);
	        	break;
	      	}
	    }
	    else if (mouse_x>236&&mouse_x<490&&mouse_y>373&&mouse_y<408)
	    {
	      	blit(multiplayer,buffer_menu,0,0,0,0,960,540);
	    }
	    else if (mouse_x>277&&mouse_x<512&&mouse_y>460&&mouse_y<521)
	    {
	      	blit(creditos,buffer_menu,0,0,0,0,960,540);
	      	if (mouse_b & 1)
	      	{
	          	int i=0;
	          	while(!key[KEY_ESC])
	          	{
	                if(i>300)
	                    blit(creditos1,buffer_menu,0,0,0,0,566,319);
	                else if(i>200)
	                    blit(creditos2,buffer_menu,0,0,0,0,566,319);
	                else
	                    blit(creditos3,buffer_menu,0,0,0,0,566,319);
	                i++;

	                if(i==400){i=0;}

	                blit(buffer_menu, screen, 0,0, 180, 130, 566, 319);
	            }
	      	}
	    }
	    else if (mouse_x>520&&mouse_x<693&&mouse_y>463&&mouse_y<521)
	    {
	      	blit(salir,buffer_menu,0,0,0,0,960,540);
	      	if (mouse_b & 1)
	        	break;
	    }
	    else{
	    	blit(idle,buffer_menu,0,0,0,0,960,540);
		}
	    masked_blit(cursor,buffer_menu,0,0,mouse_x,mouse_y,50,75);
	    blit(buffer_menu, screen, 0, 0, 0, 0, 960, 540);
    }

}

inline void config_allegro()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, BUFFER_ANCHO, BUFFER_ALTO, 0, 0);
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       	allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }
    set_volume(70, 70);
}

void iniciar_juego(){
	config_allegro();
	Juego oJuego;
	oJuego.menu();
	//oJuego.ejecutar(0);
}

// == MAIN ==============================================================

int main ()
{
	iniciar_juego();
	return 0;
}
END_OF_MAIN ()
