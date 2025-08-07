			// --- Othello. Analista Universitario en Sistemas - Programacion I. Marcos Ojeda. ---

/* +++ Conceptos clave +++

	* Inicialmente se debe solicitar el nombre de los jugadores y que color desee emplear cada uno.

	* Se debe sortear cual de los dos colores inicia.

	* A continuación debe comenzar la ronda de turnos.

	* Mientras se desarrolla la partida, se debe mostrar claramente a que jugador le toca y cuantas casillas.

	* Permitir el reseteo del juego, es decir, iniciar la partida nuevamente para los mismos jugadores, no pedir nuevamente los nombres.
	
--- Conceptos clave --- */

/* +++ Hacer +++

	* ~~Programar el spawn aleatorio. (2 fichas blancas y 2 fichas negras dispuestas en forma de "X" o dos columnas de igual color)~~

	* ~~Programar la logica del juego. (turnos, captura, gameloop)~~

--- Hacer --- */

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <unistd.h> // getpid(), system("clear")

// Constantes dedicadas a cantidad/tamaño:
#define MAX_JUGADORES 2
#define MAX_NOMBRE 20
#define TAM_TABLERO 8

// Constantes dedicadas a las configuraciones (CRUZ, COLUMNA):
#define CONFIGURACIONES 2
#define CONFIG_CRUZ 0
#define CONFIG_COLUMNA 1

// Constantes dedicadas al tablero/fichas:
#define BLANCAS 'O'
#define NEGRAS 'X'
#define POSIBLE '*'
#define VACIO '.'

// Constante dedicada al tiempo de espera de la instruccion sleep():
#define ESPERA 1
#define ESPERA_ERROR 2

// Funciones relacionadas al tablero:
void iniciarTablero(char tablero[][TAM_TABLERO]);

void mostrarTablero(char tablero[][TAM_TABLERO]);

void limpiarTablero(char tablero[][TAM_TABLERO]);

void sorteoConfiguracion(char tablero[][TAM_TABLERO]);

int jugadasPosibles(char tablero[][TAM_TABLERO], char ficha);

// Funciones relacionadas al jugador:
void solicitarJugadores(char jugadores[][MAX_NOMBRE]);

int sorteoJugadores(char jugadores[][MAX_NOMBRE], int *segundo);

void elegirColores(char jugadores[][MAX_NOMBRE], char colores[MAX_JUGADORES], int primero, int segundo);

void ganador(char jugadores[][MAX_NOMBRE], int movimientos[MAX_JUGADORES], int puntos[MAX_JUGADORES], int primero, int segundo);

// Funciones relacionadas a las fichas:
int capturarFichas(char tablero[][TAM_TABLERO], int x, int y, char ficha, int puntos[MAX_JUGADORES], int turno);

int moverFichas(char tablero[][TAM_TABLERO], char jugadores[][MAX_NOMBRE], int movimientos[MAX_JUGADORES], int puntos[MAX_JUGADORES], char ficha, int turno);

int convertirLetra(char letra);

// Funciones relacionadas al juego en si:
void juego(char jugadores[][MAX_NOMBRE]);

int revancha();

int main() 
{
	system("clear"); // Limpio la pantalla asi la terminal queda sin nada y se aprecia mejor el juego.

	int seguir_jugando;
	char jugadores[MAX_JUGADORES][MAX_NOMBRE];

	solicitarJugadores(jugadores); // Aca defini a los jugadores por unica vez. (Por si luego quieren revancha).

	do {
		juego(jugadores);

		seguir_jugando = revancha();
	} while (seguir_jugando); // Si el jugador no quiere seguir jugando entonces se termina el juego.

	return 0;
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

void limpiarTablero(char tablero[][TAM_TABLERO])
{
	for (int i = 0; i < TAM_TABLERO; i++) {
		for (int j = 0; j < TAM_TABLERO; j++) {
			if (tablero[i][j] == POSIBLE) tablero[i][j] = VACIO;
		}
	}
}

void sorteoConfiguracion(char tablero[][TAM_TABLERO])
{
	srand(getpid());
	int config_inicial = rand() % CONFIGURACIONES; // Numero aleatorio entre 0 y 1.

	if (config_inicial == CONFIG_CRUZ) { // 0.
		tablero[3][3] = BLANCAS;
		tablero[4][4] = BLANCAS;
		tablero[4][3] = NEGRAS;
		tablero[3][4] = NEGRAS;
	}
	else if (config_inicial == CONFIG_COLUMNA) { // 1.
		tablero[3][3] = BLANCAS;
		tablero[4][3] = BLANCAS;
		tablero[4][4] = NEGRAS;
		tablero[3][4] = NEGRAS;
	}
}

int jugadasPosibles(char tablero[][TAM_TABLERO], char ficha) 
{
	int hay_jugada = 0;
	char rival;

	if (ficha == BLANCAS) rival = NEGRAS;
	else rival = BLANCAS;

	for (int x = 0; x < TAM_TABLERO; x++) {
		for (int y = 0; y < TAM_TABLERO; y++) {
			if (tablero[x][y] != VACIO) continue;

			int es_valida = 0;

			for (int delta_x = -1; delta_x <= 1; delta_x++) {
				for (int delta_y = -1; delta_y <= 1; delta_y++) {
					if (delta_x == 0 && delta_y == 0) continue;

					int tmp_x = x + delta_x;
					int tmp_y = y + delta_y;
					int cantidad_rival = 0;
					while (tmp_x >= 0 && tmp_x < TAM_TABLERO && tmp_y >= 0 && tmp_y < TAM_TABLERO && tablero[tmp_x][tmp_y] == rival) { // Mientras siga viendo fichas del rival.
						cantidad_rival++;
						tmp_x += delta_x;
						tmp_y += delta_y;
					}

					if (cantidad_rival > 0 && tmp_x >= 0 && tmp_x < TAM_TABLERO && tmp_y >= 0 && tmp_y < TAM_TABLERO && tablero[tmp_x][tmp_y] == ficha) { // Si vi al menos una del rival y termina en una mia, es valida.
						es_valida = 1;
					}
				}
			}

			if (es_valida) {
				tablero[x][y] = POSIBLE;
				hay_jugada = 1;
			}
		}
	}

	return hay_jugada;
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
			sleep(ESPERA);
			break;

		case 'N': case 'n':
			colores[primero] = NEGRAS;
			colores[segundo] = BLANCAS;
			printf("\n\t\t\t\t%s prefiere negras, por lo tanto %s es blancas.\n\n", jugadores[primero], jugadores[segundo]);
			sleep(ESPERA);
			break;

		default:
			printf("\n\t\t\t\t[[ERROR]]: Por favor, solo elige entre (B)lanco o (N)egro.\n\n");
			sleep(ESPERA_ERROR);
		}

	} while (color_elegido != 'B' && color_elegido != 'b' && color_elegido != 'N' && color_elegido != 'n');
}

void ganador(char jugadores[][MAX_NOMBRE], int movimientos[MAX_JUGADORES], int puntos[MAX_JUGADORES], int primero, int segundo) 
{
	if (puntos[primero] > puntos[segundo]) {
		system("clear");
		printf("Felicidades %s, ganaste!!!\n", jugadores[primero]);
	} 

	else if (puntos[primero] < puntos[segundo]){
		system("clear");
		printf("Felicidades %s, ganaste!!!\n", jugadores[segundo]);

	} else {
		system("clear");
		printf("Empate!!!\n");
	}

	printf("\tResumen de la partida: Movimientos jugador nº1 (%s): %d,\tPuntos: %d.\tMovimientos jugador nº2 (%s): %d,\tPuntos: %d.\n\n", jugadores[primero], movimientos[primero], puntos[primero], jugadores[segundo], movimientos[segundo], puntos[segundo]);
}

int capturarFichas(char tablero[][TAM_TABLERO], int x, int y, char ficha, int puntos[MAX_JUGADORES], int turno)
{
	int captura = 0;

	char rival;

	if (ficha == BLANCAS) rival = NEGRAS;
	else rival = BLANCAS;

	for (int delta_x = -1; delta_x <= 1; delta_x++) {
		for (int delta_y = -1; delta_y <= 1; delta_y++) {
			if (delta_x == 0 && delta_y == 0) continue;

			int tmp_x = delta_x + x;
			int tmp_y = delta_y + y;
			int cantidad_rival = 0;

			while (tmp_x >= 0 && tmp_x < TAM_TABLERO && tmp_y >= 0 && tmp_y < TAM_TABLERO && tablero[tmp_x][tmp_y] == rival) {
				tmp_x += delta_x;
				tmp_y += delta_y;
				cantidad_rival++;
			}

			if (cantidad_rival > 0 && tmp_x >= 0 && tmp_x < TAM_TABLERO && tmp_y >= 0 && tmp_y < TAM_TABLERO && tablero[tmp_x][tmp_y] == ficha) {
				captura = 1;
				int xx = x + delta_x;
				int yy = y + delta_y;
				puntos[turno] += cantidad_rival;
				while (tablero[xx][yy] == rival) {
					tablero[xx][yy] = ficha;
					xx += delta_x;
					yy += delta_y;
				}
			}
		}
	}
	return captura;
}

int moverFichas(char tablero[][TAM_TABLERO], char jugadores[][MAX_NOMBRE], int movimientos[MAX_JUGADORES], int puntos[MAX_JUGADORES], char ficha, int turno)
{
	char letra;
	int x, y;
	int se_movio = 0;
	int captura;
	printf("\n%s, indica hacia donde quieres mover tu ficha (Letra y Numero): ", jugadores[turno]);
	scanf(" %c", &letra);
	scanf(" %d", &x);
	x--; // Cualquiera sea el valor de x le resto 1 para que coincida con la escala del tablero.
	y = convertirLetra(letra);

	if (x < 0 || x > TAM_TABLERO || y < 0 || y > TAM_TABLERO) {
		printf("\t\t[[ERROR]] Movimiento no valido. FUERA DE RANGO.\n\n");
		sleep(ESPERA_ERROR);
		return se_movio; /* Directamente devuelve 0 ya que el movimiento
				  * es invalido. */
	}

	if (tablero[x][y] == VACIO) { /* Si el tablero esta vacio en esa
				       * posicion entonces... */

		captura = capturarFichas(tablero, x, y, ficha, puntos, turno); /* Comprobamos si es posible 
										* capturar fichas 
										* moviendonos a esa posicion. */

		if (captura) {
			tablero[x][y] = ficha;
			movimientos[turno]++;
			se_movio = 1;
		} else {
			printf("\n[[ERROR]] No se puede poner la ficha. No captura ninguna ficha del rival.\n");
			sleep(ESPERA_ERROR);
			return se_movio;
		}
	}

	else if (tablero[x][y] == BLANCAS) {
		printf("\t\t[[ERROR]] Movimiento no valido. Esa casilla no esta vacia (HAY UNA FICHA BLANCA)\n\n");
		sleep(ESPERA_ERROR);
		se_movio = 0;
	}

	else if (tablero[x][y] == NEGRAS) {
		printf("\t\t[[ERROR]] Movimiento no valido. Esa casilla no esta vacia (HAY UNA FICHA NEGRA)\n\n");
		sleep(ESPERA_ERROR);
		se_movio = 0;
	}

	return se_movio;
}

int convertirLetra(char letra) /* Esta funcion me va a devolver la letra que
				* introduzcamos "convertida" a numero, para
				* poder ser usada como coordenada en el tablero. */
{
	int coord;
	switch (letra) {
		case 'A': case 'a':
			coord = 0;
			break;

		case 'B': case 'b':
			coord = 1;
			break;

		case 'C': case 'c':
			coord = 2;
			break;

		case 'D': case 'd':
			coord = 3;
			break;

		case 'E': case 'e':
			coord = 4;
			break;

		case 'F': case 'f':
			coord = 5;
			break;

		case 'G': case 'g':
			coord = 6;
			break;

		case 'H': case 'h':
			coord = 7;
			break;
		default:
			printf("\nPor favor, selecciona una letra valida. (A-H).\n");
			coord = -1;
		}
	return coord;
}

void juego (char jugadores[][MAX_NOMBRE])
{
	char colores[MAX_JUGADORES];
	char tablero[TAM_TABLERO][TAM_TABLERO];
	int movimientos[MAX_JUGADORES] = {0, 0};
	int puntos[MAX_JUGADORES] = {0, 0};
	int primero;
	int segundo;
	int turno;
	int se_movio;
	int jugadas_disponibles = 1;

	primero = sorteoJugadores(jugadores, &segundo);

	sleep(ESPERA); // Le doy tiempo a los jugadores a leer quien empieza.

	turno = primero;

	elegirColores(jugadores, colores, primero, segundo);

	sleep(ESPERA);

	system("clear");

	iniciarTablero(tablero);

	sorteoConfiguracion(tablero);

	while (jugadas_disponibles) {

		jugadas_disponibles = jugadasPosibles(tablero, colores[turno]);

		if (jugadas_disponibles == 0) break;

		mostrarTablero(tablero);

		limpiarTablero(tablero);

		se_movio = moverFichas(tablero, jugadores, movimientos, puntos, colores[turno], turno);

		if (se_movio) turno = (turno + 1) % 2;

		system("clear");

		printf("\n\nMovimientos jugador nº1 (%s): %d,\tPuntos: %d.\tMovimientos jugador nº2 (%s): %d,\tPuntos: %d.\n\n", jugadores[primero], movimientos[primero], puntos[primero], jugadores[segundo], movimientos[segundo], puntos[segundo]);
	}

	ganador(jugadores, movimientos, puntos, primero, segundo);

}

int revancha()
{
	int revancha;
	char opcion;

	do {
		printf("Revancha? (S)i o (N)o: ");
		scanf(" %c", &opcion);

		switch (opcion) {
			case 'S': case 's':
				  revancha = 1;
				break;

			case 'N': case 'n':
				  revancha = 0;
				  printf("\nMuchas gracias por jugar!\n");
				  sleep(ESPERA);
				  system("clear");
				break;

			default:
				printf("\n[[ERROR]] Por favor, selecciona una opcion valida (S)i o (N)o.\n");
				sleep(ESPERA_ERROR);
				system("clear");
		}
	} while (opcion != 'S' && opcion != 's' && opcion != 'N' && opcion != 'n');

	return revancha;
}
