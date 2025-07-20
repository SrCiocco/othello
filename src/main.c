/* +++ Conceptos clave +++ */

	/* Inicialmente se debe solicitar el nombre de los jugadores y que color desee emplear cada uno. */

	/* Se debe sortear cual de los dos colores inicia. */

	/* A continuación debe comenzar la ronda de turnos. */

	/* Mientras se desarrolla la partida, se debe mostrar claramente a que jugador le toca y cuantas casillas. */

	/* Permitir el reseteo del juego, es decir, iniciar la partida nuevamente para los mismos jugadores, no pedir nuevamente los nombres. */

/* --- Conceptos clave --- */

/* +++ TODO +++ */

	/* Programar el spawn aleatorio. (2 fichas blancas y 2 fichas negras dispuestas en forma de "X" o dos columnas de igual color) */

	/* Programar la logica del juego. (turnos, captura, gameloop) */

/* --- TODO --- */

#include <stdio.h>
#include <stdlib.h> /* rand(), srand() */
#include <unistd.h> /* getpid() */

#define MAX_JUGADORES 2
#define MAX_NOMBRE 20
#define MAX_COLORES 2

#define BLANCO 'B'
#define NEGRO 'N'

void solicitarJugadores(char[MAX_JUGADORES][MAX_NOMBRE]);
void elegirColores(char[MAX_JUGADORES][MAX_NOMBRE]);
int main () {
	char jugadores[MAX_JUGADORES][MAX_NOMBRE];
	solicitarJugadores(jugadores);
	elegirColores(jugadores);
	return 0;
}

void solicitarJugadores(char jugadores[MAX_JUGADORES][MAX_NOMBRE]){
	for(int i = 0; i < MAX_JUGADORES; i++){
		printf("Ingrese el nombre para el jugador nº%d: ", i + 1);
		scanf("%19s", jugadores[i]);
	}	
}

void elegirColores(char jugadores[MAX_JUGADORES][MAX_NOMBRE]){
	srand(getpid());
	int elige = rand() % 2; /* Numero aleatorio entre 0 y 1. */
	int otro;
	char color;
	printf("%s, ingresa el color que prefieras. (B)lanco o (N)egro: ", jugadores[elige]);
	scanf("%s", &color);

	for(int i = 0; i < MAX_JUGADORES; i++){
		if(i != elige){
			otro = i;
		}
	}

	switch(color){
		case 'B':
			printf("%s prefiere blancas, por lo tanto %s es negras.", jugadores[elige], jugadores[otro]);
			break;
			
		case 'N':
			printf("%s prefiere negras, por lo tanto %s es blancas.", jugadores[elige], jugadores[otro]);
			break;

		case 'b':
			printf("%s prefiere blancas, por lo tanto %s es negras.", jugadores[elige], jugadores[otro]);
			break;

		case 'n':
			printf("%s prefiere negras, por lo tanto %s es blancas.", jugadores[elige], jugadores[otro]);
			break;

		default:
			printf("Por favor, solo elige entre (B)lanco o (N)egro");
	}
}
