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
#include <unistd.h> // getpid(), system("clear")

#define MAX_JUGADORES 2
#define MAX_NOMBRE 20
#define TAM_TABLERO 8

#define CONFIGURACIONES 2
#define CONFIG_CRUZ 0
#define CONFIG_COLUMNA 1

#define BLANCAS 'O'
#define NEGRAS 'X'
#define POSIBLE '*'
#define VACIO '.'

void solicitarJugadores(char jugadores[][MAX_NOMBRE]);

int sorteoJugadores(char jugadores[][MAX_NOMBRE], int *segundo);

void elegirColores(char jugadores[][MAX_NOMBRE], char colores[MAX_JUGADORES], int primero, int segundo);

void iniciarTablero(char tablero[][TAM_TABLERO]);

int capturarFichas(char tablero[][TAM_TABLERO], int x, int y, char ficha, int puntos[MAX_JUGADORES], int turno);

int moverFichas(char tablero[][TAM_TABLERO], char jugadores[][MAX_NOMBRE], int movimientos[MAX_JUGADORES], int puntos[MAX_JUGADORES], char ficha, int turno);

void sorteoConfiguracion(char tablero[][TAM_TABLERO]);

void mostrarTablero(char tablero[][TAM_TABLERO]);

int convertirLetra(char letra);

int jugadasPosibles(char tablero[][TAM_TABLERO], char ficha);

void limpiarTablero(char tablero[][TAM_TABLERO]);

void ganador(char jugadores[][MAX_NOMBRE], int movimientos[MAX_JUGADORES], int puntos[MAX_JUGADORES], int primero, int segundo);

int main() 
{
	char jugadores[MAX_JUGADORES][MAX_NOMBRE];
	char colores[MAX_JUGADORES];
	char tablero[TAM_TABLERO][TAM_TABLERO];
	int movimientos[MAX_JUGADORES] = {0, 0};
	int puntos[MAX_JUGADORES] = {0, 0};
	int primero;
	int segundo;
	int turno;
	int se_movio;
	int jugadas_disponibles = 1;

	solicitarJugadores(jugadores);

	primero = sorteoJugadores(jugadores, &segundo);

	turno = primero;

	elegirColores(jugadores, colores, primero, segundo);
	
	iniciarTablero(tablero);

	sorteoConfiguracion(tablero);

	while (jugadas_disponibles) {

		jugadas_disponibles = jugadasPosibles(tablero, colores[turno]);

		if (jugadas_disponibles == 0) break;

		mostrarTablero(tablero);

		limpiarTablero(tablero);

		se_movio = moverFichas(tablero, jugadores, movimientos, puntos, colores[turno], turno);

		if (se_movio) turno = (turno + 1) % 2;

		printf("\n\nMovimientos jugador nº1 (%s): %d,\tPuntos: %d.\tMovimientos jugador nº2 (%s): %d,\tPuntos: %d.\n\n", jugadores[primero], movimientos[primero], puntos[primero], jugadores[segundo], movimientos[segundo], puntos[segundo]);

		}

	ganador(jugadores, movimientos, puntos, primero, segundo);

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
//	system("clear"); // Limpiar la pantalla.

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
	int config_inicial = rand() % CONFIGURACIONES;

	if (config_inicial == CONFIG_CRUZ) {
		tablero[3][3] = BLANCAS;
		tablero[4][4] = BLANCAS;
		tablero[4][3] = NEGRAS;
		tablero[3][4] = NEGRAS;
	}
	else if (config_inicial == CONFIG_COLUMNA) {
		tablero[3][3] = BLANCAS;
		tablero[4][3] = BLANCAS;
		tablero[4][4] = NEGRAS;
		tablero[3][4] = NEGRAS;
	}
}

int jugadasPosibles(char tablero[][TAM_TABLERO], char ficha) {
    int hay_jugada = 0;

    char rival;

    if (ficha == BLANCAS) rival = NEGRAS;
    else rival = BLANCAS;

    for (int fila = 0; fila < TAM_TABLERO; fila++) {
        for (int col = 0; col < TAM_TABLERO; col++) {

            if (tablero[fila][col] != VACIO) continue;

            // Revisar en las 8 direcciones
            int es_valida = 0;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {

                    if (dx == 0 && dy == 0) continue;

                    int x = fila + dx;
                    int y = col + dy;
                    int cantidad_rival = 0;

                    // Mientras siga viendo fichas del rival
                    while (x >= 0 && x < TAM_TABLERO &&
                           y >= 0 && y < TAM_TABLERO &&
                           tablero[x][y] == rival) {

                        cantidad_rival++;
                        x += dx;
                        y += dy;
                    }

                    // Si vi al menos una del rival y termina en una mía, es válida
                    if (cantidad_rival > 0 &&
                        x >= 0 && x < TAM_TABLERO &&
                        y >= 0 && y < TAM_TABLERO &&
                        tablero[x][y] == ficha) {

                        es_valida = 1;
                    }
                }
            }

            if (es_valida) {
                tablero[fila][col] = POSIBLE;
                hay_jugada = 1;
            }
        }
    }

    return hay_jugada;
}


int capturarFichas(char tablero[][TAM_TABLERO], int x, int y, char ficha, int puntos[MAX_JUGADORES], int turno)
{
	int captura = 0;

	char rival;

	if (ficha == BLANCAS) rival = NEGRAS;
	else rival = BLANCAS;

	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) continue;

			int i = dx + x;
			int j = dy + y;
			int cantidad_rival = 0;

			while (i >= 0 && i < TAM_TABLERO && j >= 0 && j < TAM_TABLERO && tablero[i][j] == rival) {
				i += dx;
				j += dy;
				cantidad_rival++;
			}
			if (cantidad_rival > 0 && i >= 0 && i < TAM_TABLERO && j >= 0 && j < TAM_TABLERO && tablero[i][j] == ficha) {
				captura = 1;
				int ii = x + dx;
				int jj = y + dy;
				puntos[turno] += cantidad_rival;
				while (tablero[ii][jj] == rival) {
					tablero[ii][jj] = ficha;
					ii += dx;
					jj += dy;
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
			return se_movio;
		}
	}

	else if (tablero[x][y] == BLANCAS) {
		printf("\t\t[[ERROR]] Movimiento no valido. Esa casilla no esta vacia (HAY UNA FICHA BLANCA)\n\n");
		se_movio = 0;
	}

	else if (tablero[x][y] == NEGRAS) {
		printf("\t\t[[ERROR]] Movimiento no valido. Esa casilla no esta vacia (HAY UNA FICHA NEGRA)\n\n");
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

	printf("\tResumen de la partida: Movimientos jugador nº1 (%s): %d,\tPuntos: %d.\tMovimientos jugador nº2 (%s): %d,\tPuntos: %d.", jugadores[primero], movimientos[primero], puntos[primero], jugadores[segundo], movimientos[segundo], puntos[segundo]);
}

