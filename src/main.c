/* +++ Conceptos clave +++

	* Inicialmente se debe solicitar el nombre de los jugadores y que color desee emplear cada uno.

	* Se debe sortear cual de los dos colores inicia.

	* A continuación debe comenzar la ronda de turnos.

	* Mientras se desarrolla la partida, se debe mostrar claramente a que jugador le toca y cuantas casillas.

	* Permitir el reseteo del juego, es decir, iniciar la partida nuevamente para los mismos jugadores, no pedir nuevamente los nombres.
	
--- Conceptos clave --- */

/* +++ TODO +++

	* Programar el spawn aleatorio. (2 fichas blancas y 2 fichas negras dispuestas en forma de "X" o dos columnas de igual color)

	* Programar la logica del juego. (turnos, captura, gameloop)

--- TODO --- */

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <unistd.h> // getpid()

#define MAX_JUGADORES 2
#define MAX_NOMBRE 20
#define TAM_TABLERO 8

#define BLANCAS 'O'
#define NEGRAS 'X'
#define VACIO '.'

void solicitarJugadores(char[][MAX_NOMBRE]);
void sorteo(char[][MAX_NOMBRE], int*, int*);
void elegirColores(char[][MAX_NOMBRE], char[MAX_JUGADORES], int, int);
void iniciarTablero(char[][TAM_TABLERO]);
void mostrarTablero(char[][TAM_TABLERO]);

int main() 
{
	char jugadores[MAX_JUGADORES][MAX_NOMBRE];
	char colores[MAX_JUGADORES];
	char tablero[TAM_TABLERO][TAM_TABLERO];
	int primero;
	int segundo;

	solicitarJugadores(jugadores);

	sorteo(jugadores, &primero, &segundo);

	elegirColores(jugadores, colores, primero, segundo);
	
	iniciarTablero(tablero);
	
	mostrarTablero(tablero);

	return 0;
}

void solicitarJugadores(char jugadores[][MAX_NOMBRE])
{
	for (int i = 0; i < MAX_JUGADORES; i++) {
		printf("Ingrese el nombre para el jugador nº%d: ", i + 1);
		scanf("%19s", jugadores[i]);
	}	
}

void sorteo(char jugadores[][MAX_NOMBRE], int *primero, int *segundo)
{
	srand(getpid());
	*primero = rand() % MAX_JUGADORES; // Numero aleatorio entre 0 y 1.

	for (int i = 0; i < MAX_JUGADORES; i++) {
		if (i != *primero) *segundo = i;
	}

	printf("Sombrero seleccionador dice: el primer turno es para %s. Luego juega %s. Doy paso a la seleccion de colores.\n", jugadores[*primero], jugadores[*segundo]);
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
			printf("%s prefiere blancas, por lo tanto %s es negras.\n\n", jugadores[primero], jugadores[segundo]);
			break;

		case 'N': case 'n':
			colores[primero] = NEGRAS;
			colores[segundo] = BLANCAS;
			printf("%s prefiere negras, por lo tanto %s es blancas.\n\n", jugadores[primero], jugadores[segundo]);
			break;

		default:
			printf("\n[[ERROR]]: Por favor, solo elige entre (B)lanco o (N)egro.\n\n");
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
	printf("\t\t\t\t    "); // Alinear las letras con los numeros.
	for (int j = 0; j < TAM_TABLERO; j++) {
		printf("%c ", 'A' + j);
	}

	printf("\n");

	for (int i = 0; i < TAM_TABLERO; i++) {
		printf("\t\t\t\t%d | ", i + 1);
		for (int j = 0; j < TAM_TABLERO; j++) {
			printf("%c ", tablero[i][j]);
		}
		printf("\n");
	}
}
