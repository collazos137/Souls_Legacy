#ifndef ESTRUCTURAS_GENERALES_H
#define ESTRUCTURAS_GENERALES_H

#include "lista.h"

typedef struct Zone Zone;
typedef struct BossAtack BossAtack;
typedef struct Boss Boss;
typedef struct Items Items;
typedef struct Statistics Statistics;
typedef struct PlayerAtacks PlayerAtacks;
typedef struct Player Player;
typedef struct Profiles Profiles;
typedef struct Clase Clase;
typedef struct BuffBatalla BuffBatalla;

struct Profiles{
	char* namePlayer;
	int lenNamePlayer;
	char* nameFile;
	int lenFile;
	Profiles* next;
};

struct Zone{
	char* name;
	int lenName;
	char* descDiscourse;
	int movingOptions;
	int* toPoint;
	int boss;
	int explored;
	char *grafFile;	
	int lenGrafFile;
};

struct BossAtack{
	char * name;
	int lenName;
	int damage;
	int impactProbability;
	int choosingProbability;
};

struct Boss{
	char * name;
	int lenName;
	char * description;
	BossAtack * atacks;
	int nAtacks;
	int hp;
	
	char* entrance;
	char* ending;

	int defeated;
	int itemKillingReward;
	int pointsKillingReward;
};

struct Items{
    char *name;
    int rareza;
    int probEncontrar;
    int staminaRec;
    int danoPot;
    int hpRec;
	int duration;
	int redDamegeBoss;
	int inmunidad;
    /* mas cosas */
	/* otras cosas */
};

struct Statistics{
	char* name;
	int lenName;
	int points;
};

struct PlayerAtacks{
	char* name;
	int lenName;

	int reactive;
	int stamina;

	int damageStatDependence;
	int * damageCalcFunction;
	int damageTerms;

	int probStatDependence;
	int * probCalcFunction;
	int probTerms;
};

struct Player{
	char* name;
	int lenName;
	List inventory;
	//int lenItems;
	int nInventory;
	Statistics* stats;
	int lenStats;
	int improvableStats;
	PlayerAtacks* atacks;
	int lenAtacks;
};

struct Clase{
	char* name;
	int lenName;
	int* stats;
};

struct BuffBatalla{
	int durationBuff;
	int lastItem;
	int fuerzaBuff;
	int reducionBoss; 
	int inmunidad;
};

#define maxInv 10


/* variables globales de zonas */
Zone* zones;
int myZone;
int nZones;

/* variables globales de bosses */
Boss * bosses;
int nBosses;

/* variable global de los items */
Items *items;
int nItems;

/* variables globales del jugador */
Player player;

/* clases de jugadores */
Clase *clases;

/* cantidad de clases de jugadores */
int nClases;

/* variable global que almacena los perfiles jugables */
Profiles *perf;

/* variable global que indica el numero de perfiles jugables */
int nProfiles;

#endif