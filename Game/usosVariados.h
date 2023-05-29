#ifndef USOS_VARIADOS_H
#define USOS_VARIADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "estructurasGenerales.h"
#include "lista.h"

int inquirirOpcion( int );
int probabilidad();

/* operaciones sobre streams */
void flushTill(FILE* , char );
void continuar();

/* operaciones sobre estructuras */
void resetInfo();

/* para el debugeo */
void printInfo();
void printProfiles();

#endif