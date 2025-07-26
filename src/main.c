/* +++ Conceptos clave +++

	* Inicialmente se debe solicitar el nombre de los jugadores y que color desee emplear cada uno.

	* Se debe sortear cual de los dos colores inicia.

	* A continuación debe comenzar la ronda de turnos.

	* Mientras se desarrolla la partida, se debe mostrar claramente a que jugador le toca y cuantas casillas.

	* Permitir el reseteo del juego, es decir, iniciar la partida nuevamente para los mismos jugadores, no pedir nuevamente los nombres.
	
--- Conceptos clave --- */

/* +++ TODO +++

	* ~~Programar el spawn aleatorio. (2 fichas blancas y 2 fichas negras dispuestas en forma de "X" o dos columnas de igual color)~~

	* Programar la logica del juego. (turnos, captura, gameloop)

--- TODO --- */

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <unistd.h> // getpid()

#define MAX_JUGADORES 2
#define MAX_NOMBRE 20
#define TAM_TABLERO 8
#define CONFIGURACIONES 2

#define BLANCAS 'O'
#define NEGRAS 'X'
#define VACIO '.'

void solicitarJugadores(char jugadores[][MAX_NOMBRE]);

int sorteoJugadores(char jugadores[][MAX_NOMBRE], int *segundo);

void elegirColores(char jugadores[][MAX_NOMBRE], char colores[MAX_JUGADORES], int primero, int segundo);

void iniciarTablero(char tablero[][TAM_TABLERO]);

void moverFichas(char tablero[][TAM_TABLERO], int movimientos[MAX_JUGADORES], int x, int y, char ficha, int turno);

void configCruz(char tablero[][TAM_TABLERO]);

void configColumna(char tablero[][TAM_TABLERO]);

void sorteoConfiguracion(char tablero[][TAM_TABLERO]);

void mostrarTablero(char tablero[][TAM_TABLERO]);

void turnoHandler(char tablero[][TAM_TABLERO], char jugadores[][MAX_NOMBRE], char colores[MAX_JUGADORES], int movimientos[MAX_JUGADORES], int *turno);

int convertirLetra(char letra);

int main() 
{
	char jugadores[MAX_JUGADORES][MAX_NOMBRE];
	char colores[MAX_JUGADORES];
	char tablero[TAM_TABLERO][TAM_TABLERO];
	int movimientos[MAX_JUGADORES] = {0, 0};
	int primero;
	int segundo;
	int turno;

	solicitarJugadores(jugadores);

	primero = sorteoJugadores(jugadores, &segundo);

	turno = primero;

	elegirColores(jugadores, colores, primero, segundo);
	
	iniciarTablero(tablero);

	sorteoConfiguracion(tablero);

	mostrarTablero(tablero);

	do {
		turnoHandler(tablero, jugadores, colores, movimientos, &turno);
		printf("\n\n\n\n\n\n\n\n\n");
		mostrarTablero(tablero);
		printf("\n\nMovimientos jugador nº1 (%s): %d\tMovimientos jugador nº2 (%s): %d\n\n", jugadores[primero], movimientos[primero], jugadores[segundo], movimientos[segundo]);
	} while (1);

	return 0;
}

void solicitarJugadores(char jugadores[][MAX_NOMBRE])
{
	for (int i = 0; i < MAX_JUGADORES; i++) {
		printf("Ingrese el nombre para el jugador nº%d: ", i + 1);
		scanf("%19s", jugadores[i]); // El limite de nombre es 20, pero le pongo 19 para darle un espacio a \0.
	}	
}

int sorteoJugadores(char jugadores[][MAX_NOMBRE], int *segundo)
{
	srand(getpid());
	int primero = rand() % MAX_JUGADORES; // Numero aleatorio entre 0 y 1.
	for (int i = 0; i < MAX_JUGADORES; i++) {
		if (i != primero) *segundo = i;
	}

	printf("\n\t\t\t\tSombrero seleccionador dice: el primer turno es para %s. Luego juega %s. Doy paso a la seleccion de colores.\n\n", jugadores[primero], jugadores[*segundo]);

	return primero;
}

void elegirColores(char jugadores[][MAX_NOMBRE], char colores[MAX_JUGADORES], int primero, int segundo)
{
	char color_elegido;

	do {
		printf("%s, ingresa el color que prefieras. (B)lanco o (N)egro: ", jugadores[primero]);
		scanf(" %c", &color_elegido);

	switch (color_elegido) {
		case 'B': case 'b':
			colores[primero] = BLANCAS;
			colores[segundo] = NEGRAS;
			printf("\n\t\t\t\t%s prefiere blancas, por lo tanto %s es negras.\n\n", jugadores[primero], jugadores[segundo]);
			break;

		case 'N': case 'n':
			colores[primero] = NEGRAS;
			colores[segundo] = BLANCAS;
			printf("\n\t\t\t\t%s prefiere negras, por lo tanto %s es blancas.\n\n", jugadores[primero], jugadores[segundo]);
			break;

		default:
			printf("\n\t\t\t\t[[ERROR]]: Por favor, solo elige entre (B)lanco o (N)egro.\n\n");
		}

	} while (color_elegido != 'B' && color_elegido != 'b' && color_elegido != 'N' && color_elegido != 'n');
}

void iniciarTablero(char tablero[][TAM_TABLERO])
{
		for (int i = 0; i < TAM_TABLERO; i++) {
			for (int j = 0; j < TAM_TABLERO; j++) {
				tablero[i][j] = VACIO;
			}
		}
}

void mostrarTablero(char tablero[][TAM_TABLERO])
{
	printf("\t\t\t\t    A B C D E F G H\n"); // Alinear las letras con los numeros.

	for (int i = 0; i < TAM_TABLERO; i++) {
		printf("\t\t\t\t%d | ", i + 1); // Alinear los numeros e imprimirlos
		for (int j = 0; j < TAM_TABLERO; j++) {
			printf("%c ", tablero[i][j]);
		}
		printf("\n");
	}
}

void sorteoConfiguracion(char tablero[][TAM_TABLERO])
{
	srand(getpid());
	int configuracion_inicial = rand() % CONFIGURACIONES;

	switch (configuracion_inicial) {
		case 0:
			configCruz(tablero);
			break;
		case 1:
			configColumna(tablero);
			break;
	}
}

void moverFichas(char tablero[][TAM_TABLERO], int movimientos[MAX_JUGADORES], int x, int y, char ficha, int turno)
{
	switch (tablero[x][y]) {
		case VACIO:
			tablero[x][y] = ficha;
			movimientos[turno]++;
			break;
		default:
			printf("\t\t[[ERROR]] Movimiento no valido. Esa casilla no esta vacia\n\n");
	}
}

void turnoHandler(char tablero[][TAM_TABLERO], char jugadores[][MAX_NOMBRE], char colores[MAX_JUGADORES], int movimientos[MAX_JUGADORES], int *turno)
{
	char letra;
	int x, y;
		printf("\n%s, indica hacia donde quieres mover tu ficha (Letra y Numero): ", jugadores[*turno]);
		scanf(" %c", &letra);
		scanf(" %d", &x);
		x--;
		y = convertirLetra(letra);
		moverFichas(tablero, movimientos, x, y, colores[*turno], *turno);
		*turno = (*turno + 1) % 2;
}
void configCruz(char tablero[][TAM_TABLERO])
{
	tablero[3][3] = BLANCAS;
	tablero[4][4] = BLANCAS;
	tablero[4][3] = NEGRAS;
	tablero[3][4] = NEGRAS;
}

void configColumna(char tablero[][TAM_TABLERO])
{
	tablero[3][3] = BLANCAS;
	tablero[4][3] = BLANCAS;
	tablero[4][4] = NEGRAS;
	tablero[3][4] = NEGRAS;
}

int convertirLetra(char letra)
{
	switch (letra) {
		case 'A': case 'a':
			return 0;
			break;

		case 'B': case 'b':
			return 1;
			break;

		case 'C': case 'c':
			return 2;
			break;

		case 'D': case 'd':
			return 3;
			break;

		case 'E': case 'e':
			return 4;
			break;

		case 'F': case 'f':
			return 5;
			break;

		case 'G': case 'g':
			return 6;
			break;

		case 'H': case 'h':
			return 7;
			break;
		default:
			printf("\nPor favor, selecciona una letra valida. (A-H).\n");
	}
	return 0;
}
