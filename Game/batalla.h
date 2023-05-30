#ifndef BATALLA_H
#define BATALLA_H

#include <stdio.h>
#include <stdlib.h>

#include "estructurasGenerales.h"
#include "lista.h"
#include "usosVariados.h"
#include "manejoArchivos.h"

typedef struct BattleAtack BattleAtack;

struct BattleAtack{
	int stamina;
	int reactive;
	int prob;
	int damage;
	char* name;
};

/* funciones utilizadas en la batalla */
int batalla(int );
void descontarHp(int * , int * , int , int , char *, int );
int turnoPropio(BattleAtack* , int* , int*, int*, BuffBatalla* , char*, int );
int turnoBoss(int, int );
void inventarioBatalla(BattleAtack*, int*, int*, BuffBatalla*);
void tableroBatalla(BattleAtack*, int*, int*, char *, int);

/* funciones para calcular el daño, hp y demas caracteristicas de la batalla que dependen de las estadisticas */
BattleAtack* playerStatisticsToBattleAtack();
int calcHp(int puntos);
int calcStamina(int puntos);
int calcRecuperacionStamina(int puntos);

#endif