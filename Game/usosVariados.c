#include "usosVariados.h"

/*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- operaciones extrañas -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

/*
Nombre: continuar
Descripcion: resetea la pantalla.
*/
void continuar(){
	fflush(stdin);
	printf("Presiona enter para continuar...");
	getchar();
	system("cls");
}

/*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- operaciones sobre las estructuras -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

/*
Nombre: resetInfo
Descripcion: resetea la informacion referida a las zonas exploradas y los bosses derrotados. Esta
				pensado para cuando se quiera volver a crear una partida.
*/
void resetInfo(){
	myZone = 0;
	int i;

	for ( i = 0 ; i < nZones ; ++i ){
		zones[i].explored = 0;
	}

	for ( i = 0 ; i < nBosses ; ++i ){
		bosses[i].defeated = 0;
	}

	player.nInventory = 0;

	kill(&player.inventory);

	player.improvableStats = 0;
	for ( i = 0 ; i < player.lenStats ; ++i ){
		(player.stats[i]).points = 0;
	}

}
/*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- operaciones sobre streams -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*
Nombre: flushTill
Entrada: lo de FILE. un caracter 'comp'
Descripcion: saca caracteres de 'inputStream' hasta que encuentra a 'comp'
*/
void flushTill(FILE* inputStream, char comp){
	char c;
	do {
		c = getc(inputStream);
	} while (c != comp);
}
/*
Nombre: probabilidad
Descripcion: genera un numero aleatorio entre 0 y 99
*/
int probabilidad(){
	int ans;
	ans = rand() % 100;
	return ans;
}
/*
Nombre: inquirirOpcion
Descripcion: le pregunta al usuario por una opcion entre 0 y maximo. Lo hace cuantas veces sea 
				necesario.
*/
int inquirirOpcion(int maximo){
	int opcion;
	do {
		printf(" > ");
		scanf("%d",&opcion);		
	} while ((opcion < 0) || (opcion >= maximo));
	return opcion;
}
/*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- operaciones para debugear -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

/*
Nombre: printProfiles()
Descripcion: imprime la informacion relacionada a los perfiles
*/
void printProfiles(){
	Profiles* tmp = perf;
	while ( tmp != NULL ){
		printf( "%s - %s\n", tmp->namePlayer, tmp->nameFile);
		tmp = tmp->next;
	}
}


/*
nombre: printInfo
Descripcion: visualiza la informacion leida.
*/
void printInfo(){
	int i, j;
	printf("         ||zonas||\n");
	printf("%d\n",nZones);
	for (i = 0 ; i < nZones ; ++i){
		printf("%s\n",zones[i].name);
		printf("%20d\n",zones[i].movingOptions);
		for (j = 0 ; j < zones[i].movingOptions ; ++j){
			printf("%d | ",zones[i].toPoint[j]);
		}
		printf("\n%d\n\n",zones[i].boss);
	}

	printf("             ||bosses||\n");

	for (i = 0 ; i < nBosses ; ++i){
		printf("%s\n",bosses[i].name);
		printf("%s", bosses[i].description);
		printf("\ndefeated = %d\n",bosses[i].defeated);
		printf("killing points = %d\n",bosses[i].pointsKillingReward);
		/*j = bossses[i].itemKillingReward;
		if ( j != -1 ){
			printf("| killing item %s\n", items[ j ].name );
		} else {
			printf(" | There is no killiing item\n");
		}*/
		printf("%d\n",bosses[i].hp);

		printf("LOs ataques:       %d\n", bosses[i].nAtacks );
		for (j = 0 ; j < bosses[i].nAtacks ; ++j){
			printf("%s | dano %d | probImp %d | probElec %d|\n",(bosses[i].atacks[j]).name, (bosses[i].atacks[j]).damage, (bosses[i].atacks[j]).impactProbability,(bosses[i].atacks[j]).choosingProbability);
		}

		printf("%s\n",bosses[i].entrance);
		printf("%s\n\n",bosses[i].ending);

	}

	printf("                || inventario || - %d\n", nItems );
	for ( i = 0; i < nItems ; ++i ){
		printf("%s\n",(items[i]).name);
		printf("rareza %d | probabilidad %d | Stamina %d | fuerza %d | vida %d\n", items[i].rareza, (items[i]).probEncontrar, (items[i]).staminaRec, (items[i]).danoPot, (items[i]).hpRec );
	}

	printf("                 || player ||\n");
	printf("%d %s\n",player.lenName,player.name);
	printf("estadisticas\n");
	for ( i = 0 ; i < player.lenStats ; ++i){
		printf("%s | puntos = %d\n",player.stats[i].name,player.stats[i].points);
	}
	printf("items - %d\n", player.nInventory );
	Node *tmp = (player.inventory).firts;
	while( tmp != NULL ){
		printf("%s\n",(items[tmp->date]).name);
		tmp = tmp->next;
	}
	
	printf("ataques\n");
	for (i = 0; i < player.lenAtacks ; ++i){
		printf("%s | reactive %d| stamina %d | ",player.atacks[i].name, player.atacks[i].reactive, player.atacks[i].stamina);
		for ( j = 0 ; j < player.atacks[i].damageTerms ; ++j ){
			printf("%d ",player.atacks[i].damageCalcFunction[j]);
		}
		printf("| ");
		for ( j = 0 ; j < player.atacks[i].probTerms ; ++j ){
			printf("%d ",player.atacks[i].probCalcFunction[j]);
		}
		printf("\n");
	}
}

