#ifndef MANEJO_ARCHIVOS_H
#define MANEJO_ARCHIVOS_H

#define lenPredFile 38

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructurasGenerales.h"
#include "lista.h"
#include "usosVariados.h"

/* Indica el archivo en donde encontraremos almacenados los perfiles */
char *allProfiles; 

/* Indica el archivo en donde estara la informacion no mutable */
char *nameNonMutable;

/* Variable global que indica el archivo en donde se encuentra la informacion relacionada al perfil actual */
char* profileFile;

char* profileFilePrefix;
char* profileFileExtension;

/* operaciones de lectura sobre archivos no-mutables */
void readNonMutableFile( );
void readNonMutableZones( FILE * );
void readNonMutableBossses( FILE * );
void readNonMutableItems( FILE * );
void readBossAtack( BossAtack *, FILE * );
void readNonMutablePlayer( FILE * );
void readNonMutableClass( FILE * );
void readDiscourse( char * );
void readGrafix( char * );


/* operaciones sobre perfiles */
void readProfiles( char * );
void modifyEnding( char * );
void writeProfiles( char * );
void readGame( char * );
void overwriteGame( char * );
void deleteGame();


#endif

/*
		Estructura del archivo 'informacionNoMutable.txt'
Un entero que indica cuantas zonas hay.
Por cada zona:  Un entero que indica la cantidad de caracteres del nombre de la zona, un espacio y 
				el nombre de la zona; un entero que indica a cuantas zonas apunta, seguido del indice 
				de las zonas a las que esta conectado. Finalmente, un entero que toma -1 si no hay ningun 
				boss en esta zona, o el indice del boss segun la aparicion del boss en el archivo de bosses.
			Nota: el indice comienza a contar desde 0.

Un entero que indica cuantos bosses hay.
Por cada boss: un entero que indica los caracteres del nombre del boss, seguido de un espacio y 
				el nombre del boss. Un entero que indica los puntos que gana el jugador al derrotar a este
				boss, seguido de otro entero que representa el indice del objeto que obtiene el jugador al
				derrotarlo ( -1 significa que no hay ningun objeto-recompensa ). Un entero que indica el hp 
				del boss. La descripcion del boss,
				todo en una unica linea. Un entero que indica el tamaño del nombre del archivo donde 
				se guarda la entrada dramatica del boss, seguido por un espacio y el nombre del 
				archivo. Despues un entero que indica los ataques del boss.
				Por cada ataque: un entero que indica el tamaño del nombre del ataque, seguido por
				un espacio y el nombre del ataque. Despues vienen tres enteros separados por espacios
				que representan: el daño del ataque, la probabilidad de realizar el ataque con exito
				(medido de 0 a 100 con 100 siendo imposible fallar y 0 siempre falla), y la probabilidad
				de que el boss escoga este ataque.

un entero que indica cuantos objetos hay.
Por cada objeto: Un entero que indica el tamaño del nombre del objeto, seguido de un espacio y el nombre del
					objeto. En otra linea, un numero que indica la rareza del objeto, un espacio y la probabilidad
					de encontrarlo al explorar. En otra linea, nos encontramos con el porcentaje de stamina que
					recupera, el porcentaje de daño que potencia y el porcentaje de vida que recupera (todos 
					separados por espacios).

un Entero que indica cuantas estadisticas hay.
Por cada estadistica: Un entero que indica el tamaño del nombre de la estadistica, seguido de un espacio y
						el nombre de la estadistica.


un entero que indica los ataques del jugador.
Por cada ataque: Un entero que indica el tamaño del nombre del ataque, seguido de un espacio y el nombre del
					ataque. Un entero que toma 0 si el ataque es sencillo (realizo el daño y ya) o 1 si es
					reactivo (depende de esquivar el ataque del boss). Un entero que indica el stamina requerido
					para realizar el ataque. Un entero que indica el indice (segun aparicion) de la estadisticca
					de la que depende el daño, seguido de un entero que indica cuantos terminos tendra la funcion
					polinomica con la que se calculara el daño y los coeficientes (del termino constante al que 
					posea el mayor exponente).
					Lo mismo para el daño se aplica para calcular la probabilidad de impacto.
*/

/*
Estructura archivo 'perfiles.txt':

Tres caracteres numericos seguidos que indican el ultimo sufijo usado en un archivo de perfil .souls
Un entero que indica la cantidad de perfiles disponibles
Por cada perfil: un numero que indica el tamaño del nombre del perfil seguido de un espacio y el nombre del perfil.
					Un numero que indica el tamaño del nombre del archivo del perfil seguido de un espacio y el
					nombre del archivo del perfil.
*/
/*
Estructura archivos .souls:

Un entero que indica la zona en la que se encuentra el jugador.

Una fila de enteros que: si la poscicion en la fila es 'h', entonces este valor es 1 si ya se ha explorado la 
							zona 'k'. El valor es 0 de lo contrario.

Una fila de enteros que: si la poscicion en la fila es 'k', entonces este valor es 1 si el boss 'k' ya ha sido vencido
							y 0 de lo contrario.

Un entero que indica los puntos de habilidad que aun no han sido asignados.
Una fila de enteros que: si la poscicion en la fila es 'j', entonces este valor respresenta el nivel que tiene la
							estadistica 'j'


un entero 'h' que indica los elementos en el inventario que tiene el jugador.
Una fila de 'h' enteros que representa el indice del objeto que el jugador posee.

Sobre los archivos de discursos:
Estos son archivos de texto que contienen parte del lore del juego.
Existen varios caracteres reservados que ayudan a personalizar el como son leidas estas 'cinematicas':
	'$' : Este imprime en su lugar el nombre completo del personaje (como lo halla ingresado el jugador).
	'|' : Este hace una pausa en la que le pide un caracter cualquiera de stdin al jugador
	'&' : Este realiza una invocacion a 'continuar()' de usosVariados.h 
*/