#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructurasGenerales.h"
#include "lista.h"
#include "usosVariados.h"
#include "manejoArchivos.h"
#include "batalla.h"

/* esta operacion aun no esta terminada */
int getName(char ** );

/* menu inicial (cuando se cargan los perfiles) */
void bigMenu( char * );
void createNewGame ( char * );
void profileSelectionMenu ( );
void choseClass();

/* menu principal */
void menuPrincipal();

/* explorar */
void antorcha();
void objetos();
void explorar();
void encontarPuntoHabilidad();

/* inventario */
void mostrarInventario();

/* bestiario */ 
void bestiario();
void imprimirDescripcion(char * );

/* estadisticas */
void menuEstadisticas();
void mejorarEstadisticas ();

/* avanzar */
void moverseEntreZonas ();

#endif