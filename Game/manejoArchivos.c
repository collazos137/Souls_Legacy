#include "manejoArchivos.h"

/*
#########################################################################################################################
#########################################################################################################################
###################################### Operaciones sobre perfiles #######################################################
#########################################################################################################################
#########################################################################################################################
*/
/*
Nombre: writeProfiles
Descripcion: sobreescribe el archivo que guarda los perfiles
*/
void writeProfiles( char* ending ){	
	FILE* outputFile = fopen( allProfiles, "w");	
	if ( outputFile == NULL ){	
		fprintf( stderr , "Error: el archivo de los perfiles |%s| no pudo ser leido.\n", allProfiles );	
	}	
	Profiles *tmp = perf;	
	if ( nProfiles == 0 ){	
		ending[0] = '0';	
		ending[1] = '0';	
		ending[2] = '0';	
	}	
	fprintf( outputFile, "%s\n%d\n\n", ending, nProfiles );	
	while ( tmp != NULL ){	
		fprintf(outputFile, "%d %s\n", tmp->lenNamePlayer, tmp->namePlayer);	
		fprintf( outputFile, "%d %s\n\n", tmp->lenFile, tmp->nameFile );	
		tmp = tmp->next;	
	}	
	fclose( outputFile );	
}

/*
Nombre: readProfiles
Descripcion: lee el archivo que guarda los perfiles
*/
void readProfiles(char* ending ){	
	FILE* inputFile = fopen(allProfiles, "r");	
	if ( inputFile == NULL ){	
		fprintf( stderr , "Error: el archivo de los perfiles |%s| no pudo ser leido.\n", allProfiles );	
	}	
	fgets(ending, 4, inputFile);	
	ending[3] = '\0';	
	fscanf(inputFile, "%d", &nProfiles );	
	/*printf("ending = %s - otro %d\n",ending,nProfiles);*/	
	Profiles* tmp;	
	int i;	
	char t;	
	for ( i = 0 ; i < nProfiles ; ++i ){	
		tmp = (Profiles*) malloc ( sizeof ( Profiles ) );	
		fscanf(inputFile, "%d", &tmp->lenNamePlayer);	
		t = getc ( inputFile );	
		tmp->namePlayer = (char*) malloc ( sizeof ( char ) * ( tmp->lenNamePlayer ) );	
		fgets(tmp->namePlayer, tmp->lenNamePlayer, inputFile );	
		/*printf("%d %s\n",tmp->lenNamePlayer,tmp->namePlayer);*/	
		fscanf( inputFile, "%d", &tmp->lenFile );	
		t = getc ( inputFile );	
		tmp->nameFile = (char*) malloc ( sizeof ( char ) * ( tmp->lenFile ) );	
		fgets ( tmp->nameFile, tmp->lenFile, inputFile );	
		tmp->next = perf;	
		perf = tmp;	
	}	
	fclose(inputFile);	
}
/*
Nombre: modifyEnding
Entrada: una cadena 'ending' que representa los digitos que va a tener el archivo del jugador antes de la extension
Descripcion: Modifica estos digitos de manera que se minimizen las repeticiones
*/
void modifyEnding( char* ending){
	int flag = 1, i = 2;
	while ( i >= 0 && flag ){
		flag = 0;
		++ending[i];
		if ( ending[i] > '9' ){
			ending[i] = '0';
			flag = 1;
			--i;
		}
	}
}

/*
Nombre: overwriteGame
Entrada: una cadena 'nameFile' que representa el nombre del archivo mutable
Descripcion: sobreescribe el archivo mutable dado por la cadena 'nameFile'.
*/
void overwriteGame(char* nameFile){
	FILE* outputFile = fopen(nameFile,"w");

	//FILE* outputFile = stdout;

	if ( outputFile == NULL ){
		fprintf( stderr , "Error: el archivo del jugador |%s| no pudo ser sobreescrito.\n", nameFile );
	}
	
	fprintf(outputFile, "%d\n\n", myZone);
	int i;

	for ( i = 0 ; i < nZones ; ++i ){
		fprintf(outputFile, "%d ", zones[i].explored);
	}
	fprintf(outputFile, "\n\n");

	for ( i = 0 ; i < nBosses ; ++i ){
		fprintf(outputFile, "%d ", bosses[i].defeated);
	}
	fprintf(outputFile, "\n\n");

	fprintf(outputFile, "%d\n", player.improvableStats);
	for ( i = 0 ; i < player.lenStats ; ++i ){
		fprintf(outputFile, "%d ", (player.stats[i]).points );
	}
	fprintf(outputFile, "\n\n");

	fprintf(outputFile, "%d\n", player.nInventory);
	/*printf("%d\n",player.nItems);*/
	Node* tmp = (player.inventory).firts;
	for ( i = 0 ; i < player.nInventory ; ++i ){
		fprintf(outputFile, "%d ", tmp->date );
		tmp = tmp->next;
	}

	fclose(outputFile);
}

/*
Nombre: readGame
Entrada: una cadena 'nameFile' que almacena el nombre del archivo de mutables
Descripcion: lee toda la informacion del archivo de mutables.
*/
void readGame(char * nameFile){
	FILE* inputFile = fopen(nameFile, "r");
	if ( inputFile == NULL ){
		fprintf( stderr , "Error: el archivo del jugador |%s| no pudo ser leido.\n", nameFile );
	}

	fscanf(inputFile, "%d", &myZone);

	int i, h;
	char t;

	for ( i = 0 ; i < nZones ; ++i ){
		fscanf(inputFile, "%d", &zones[i].explored);
	}

	for ( i = 0 ; i < nBosses ; ++i ){
		fscanf(inputFile, "%d", &bosses[i].defeated);
	}

	fscanf(inputFile, "%d", &player.improvableStats);
	for ( i = 0 ; i < player.lenStats ; ++i ){
		fscanf(inputFile, "%d", &(player.stats[i]).points );
	}

	fscanf(inputFile, "%d", &player.nInventory );
	kill( &player.inventory );
	for ( i = 0 ; i < player.nInventory ; ++i ){
		fscanf(inputFile, "%d", &h);
		append(&player.inventory, h);
	}

	fclose(inputFile);
}
/*	
Nombre: deleteGame	
Descripcion: borra el juego del archivo de perfiles y de la estructura 'perf'	
*/	
void deleteGame(){	
	if ( perf == NULL ){	
		fprintf(stderr, "Error: no se puede eliminar un perfil si no hay perfiles\n");	
	} else if ( strcmp( player.name, perf->namePlayer ) == 0 ){	
		Profiles *tmp = perf;	
		perf = perf->next;	
		free( tmp );	
	} else {	
		Profiles *tmp = perf;	
		while ( tmp->next != NULL && strcmp( player.name, (tmp->next)->namePlayer ) != 0 ){	
			tmp = tmp->next;	
		}	
		if ( tmp == NULL ){	
			fprintf( stderr, "Error: El perfil del jugador %s no fue encontrado en la estructura de perfiles.\n", player.name );	
		} else {	
			Profiles *toErase = tmp->next;	
			tmp->next = toErase->next;	
			free( toErase );	
		}	
	}	
	writeProfiles( allProfiles );	
}
/*
#########################################################################################################################
#########################################################################################################################
###################################### Operaciones sobre archivos no-mutables (solo lectura) ############################
#########################################################################################################################
#########################################################################################################################
*/
/*
nombre: readDiscourse
Entrada: una cadena 'nameFile' que indica el nombre del archivo que contiene la entrada dramatica
Descripcion: Imprime el archivo, haciendo pausas en los saltos de linea..
*/
void readDiscourse(char* nameFile){
	/*printf("%s|\n",nameFile);*/
	FILE* inputFile = fopen(nameFile, "r");
	if ( inputFile == NULL ){
		fprintf( stderr , "Error: el archivo no-mutable |%s| no pudo ser leido.\n", nameFile );
	}
	char c;
	while (fscanf(inputFile, "%c",&c) != EOF ){
		if (c == '$'){
			printf("%s", player.name);
		} else if (c == '|'){
			c = getc( stdin );
		} else if ( c == '&' ){
			continuar();
		} else {
			printf("%c",c);
		}
	}
	system("cls");
	fclose(inputFile);
}


/*
Nombre: readNonMutableFile
Entrada: una cadena 'nameFile'
Descripcion: lee todo el archivo de informacion no mutable (este es el guardado en 'nameFile').
*/
void readNonMutableFile(){
	FILE* inputFile = fopen(nameNonMutable, "r");
	if ( inputFile == NULL ){
		fprintf( stderr , "Error: el archivo no mutable |%s| no pudo ser leido.\n", nameNonMutable );
	}
	readNonMutableZones(inputFile);
	readNonMutableBossses(inputFile);
	readNonMutableItems( inputFile );
	readNonMutablePlayer(inputFile);
	readNonMutableClass(inputFile);
	fclose(inputFile);
}

void readNonMutableClass(FILE* inputFile){
	fscanf(inputFile, "%d", &nClases);
	
	clases = (Clase*) malloc ( sizeof ( Clase ) * nClases ); 
	
	int i, j;
	char t;
	for ( i = 0 ; i < nClases ; ++i ){
		fscanf(inputFile, "%d", &((clases[i]).lenName) );
		(clases[i]).name = (char*) malloc ( sizeof ( char ) * (clases[i]).lenName );
		(clases[i]).stats = (int*) malloc ( sizeof ( int ) * player.lenStats);
		t = getc(inputFile);
		fgets( (clases[i]).name, (clases[i]).lenName, inputFile );
		
		for ( j = 0 ; j < player.lenStats ; ++j ){
			fscanf(inputFile, "%d", &((clases[i]).stats[j]) );
		}
	}
}

/*
Nombre: readNonMutableItems
Entrada: un stream 'inputFile' del que se van a leer los datos.
Descripcion: lee toda la informacion que va a ir a la estructura 'items'.
*/
void readNonMutableItems( FILE* inputFile ){
	int len, i;
	char t;

	fscanf(inputFile, "%d", &nItems);

	items = (Items*) malloc ( sizeof ( Items ) * nItems );

	for ( i = 0 ; i < nItems ; ++i ){
		fscanf( inputFile, "%d", &len );
		items[i].name = (char*) malloc ( sizeof ( char ) * len );
		t = getc(inputFile);
		fgets( (items[i]).name, len, inputFile );

		fscanf( inputFile, "%d", &items[i].rareza );
		fscanf( inputFile, "%d", &items[i].probEncontrar );
		fscanf( inputFile, "%d", &items[i].duration);
		fscanf( inputFile, "%d", &items[i].staminaRec );
		fscanf( inputFile, "%d", &items[i].danoPot );
		fscanf( inputFile, "%d", &items[i].hpRec );
		fscanf( inputFile, "%d", &items[i].redDamegeBoss);
		fscanf( inputFile, "%d", &items[i].inmunidad);
	}
}


/*
Nombre: readNonMutablePlayer
Entrada un stream 'inputFile' del que se van a leer los datos.
Descripcion: lee lo que se encuentra en el archivo de informacion no mutable referido a la informacion del jugador.
				Aqui se incluye informacion sobre items, estadisticas y ataques.
*/
void readNonMutablePlayer(FILE* inputFile){

	int len;

	fscanf(inputFile, "%d", &player.lenStats);

	player.stats = (Statistics*) malloc ( sizeof ( Statistics ) * player.lenStats );
	int i, j;
	char t;

	for ( i = 0 ; i < player.lenStats ; ++i ){
		fscanf(inputFile, "%d", &((player.stats[i]).lenName) );
		(player.stats[i]).name = (char*) malloc ( sizeof ( char ) * ( player.stats[i]).lenName + 1 );
		t = getc(inputFile);
		fgets( (player.stats[i]).name, (player.stats[i]).lenName, inputFile );
	}

	fscanf(inputFile, "%d", &player.lenAtacks);
	player.atacks = (PlayerAtacks*) malloc( sizeof ( PlayerAtacks ) * player.lenAtacks );

	for ( i = 0 ; i < player.lenAtacks ; ++i ){
		fscanf(inputFile, "%d", &((player.atacks[i]).lenName));
		(player.atacks[i]).name = (char*) malloc ( sizeof ( char ) * ( player.atacks[i]).lenName + 1 );
		t = getc(inputFile);
		fgets( (player.atacks[i]).name, (player.atacks[i]).lenName, inputFile );
		fscanf(inputFile, "%d", &(player.atacks[i]).reactive);
		fscanf(inputFile, "%d", &(player.atacks[i]).stamina);

		fscanf(inputFile, "%d", &(player.atacks[i]).damageStatDependence);
		fscanf(inputFile, "%d", &(player.atacks[i]).damageTerms);
		(player.atacks[i]).damageCalcFunction = (int*) malloc( sizeof( int ) * (player.atacks[i]).damageTerms );
		for ( j = 0 ; j < (player.atacks[i]).damageTerms ; ++j ){
			fscanf(inputFile, "%d", &((player.atacks[i]).damageCalcFunction[j]) );
		}

		fscanf(inputFile, "%d", &(player.atacks[i]).probStatDependence);
		fscanf(inputFile, "%d", &(player.atacks[i]).probTerms);
		(player.atacks[i]).probCalcFunction = (int*) malloc( sizeof( int ) * (player.atacks[i]).probTerms );
		for ( j = 0 ; j < (player.atacks[i]).probTerms ; ++j ){
			fscanf(inputFile, "%d", &((player.atacks[i]).probCalcFunction[j]) );
		}
	}

}

/*
Nombre: readNonMutableBosses
Entrada: un stream 'inputFile' del que se van a leer los datos.
Descripcion: lee todo lo relacionado a los bosses en el archivo de informacion no mutable.
*/
void readNonMutableBossses(FILE * inputFile){
	fscanf(inputFile, "%d", &nBosses);
	bosses = (Boss*) malloc( sizeof ( Boss ) * nBosses );

	/*printf("%d\n",nBosses);*/

	int i,j, len;
	char t;
	for ( i = 0 ; i < nBosses ; ++i){
		fscanf(inputFile, "%d", &bosses[i].lenName);
		bosses[i].name = (char*) malloc ( sizeof ( char ) * (bosses[i].lenName + 1));

		t = getc(inputFile);
		fgets(bosses[i].name, bosses[i].lenName, inputFile);

		/*printf("Nombre boss: %s\n", bosses[i].name);*/

		fscanf(inputFile, "%d", &bosses[i].pointsKillingReward);
		fscanf(inputFile, "%d", &bosses[i].itemKillingReward);


		fscanf(inputFile, "%d", &bosses[i].hp);

		fscanf(inputFile, "%d", &len);
		t = getc(inputFile);
		bosses[i].description = (char*) malloc ( sizeof ( char ) * (len + 1) );
		fgets(bosses[i].description, len, inputFile);

		/*printf("description: %s\n", bosses[i].description );*/

		fscanf(inputFile, "%d", &len);
		bosses[i].entrance = (char*) malloc( sizeof ( char ) * ( len + 1) );
		t = getc(inputFile);
		fgets( bosses[i].entrance, len, inputFile );

		/*printf("archivos entrada: %s\n", bosses[i].entrance);*/

		fscanf( inputFile, "%d", &len );
		/*printf("%d\n", len);*/
		bosses[i].ending = (char*) malloc( sizeof ( char ) * ( len + 1) );
		t = getc(inputFile);
		fgets( bosses[i].ending, len, inputFile );

		/*printf("archivos salida: %s\n", bosses[i].ending);*/


		fscanf(inputFile,"%d",&bosses[i].nAtacks);
		bosses[i].atacks = (BossAtack*) malloc ( sizeof ( BossAtack ) * bosses[i].nAtacks );

		/*printf("%d\n",bosses[i].nAtacks);*/

		for ( j = 0 ; j < bosses[i].nAtacks ; ++j){
			readBossAtack(&(bosses[i].atacks[j]), inputFile);
		}
	}
}
/*
Nombre: readBossAtack
Entrada: un stream 'inputFile' del que se van a leer los datos.
Descripcion: lee la informacion relacionada a 1 ataque de 1 boss.
*/
void readBossAtack(BossAtack* bossAtack, FILE* inputFile){
	fscanf(inputFile, "%d", &bossAtack->lenName);
	bossAtack->name = (char*) malloc ( sizeof ( char ) * (bossAtack->lenName + 1) );
	bossAtack->name[0] = getc(inputFile);
	fgets(bossAtack->name, bossAtack->lenName, inputFile);

	/*printf("el nombre ataque: %s\n", bossAtack->name);*/

	fscanf(inputFile, "%d", &bossAtack->damage);
	fscanf(inputFile, "%d", &bossAtack->impactProbability);
	fscanf(inputFile, "%d", &bossAtack->choosingProbability);

	/*printf("%d %d %d\n",bossAtack->damage, bossAtack->impactProbability, bossAtack->choosingProbability);*/
}
/*	
Nombre: readNonMutableZones	
Entrada: un stream 'inputFile' del que se van a leer los datos.	
Descripcion: lee todo lo relacionado a las zonas en el archivo de informacion no mutable.	
*/	
void readNonMutableZones(FILE * inputFile){	
	fscanf(inputFile, "%d",&nZones);	
	/*printf("%d\n",nZones);*/	
	zones = (Zone*) malloc( sizeof( Zone ) * nZones );	
	int i,j, len;	
	char t;	
	for ( i = 0 ; i < nZones ; ++i){	
		fscanf(inputFile, "%d", &zones[i].lenName);	
		zones[i].name = (char*) malloc( sizeof( char ) * (zones[i].lenName + 1 ) );	
		t = getc(inputFile);	
		fgets(zones[i].name, zones[i].lenName, inputFile);	
		/*printf("zoneName: %s|\n", zones[i].name );*/	
		fscanf(inputFile, "%d", &len);	
		zones[i].descDiscourse = (char*) malloc( sizeof( char ) * (len+ 1 ) );	
		t = getc(inputFile);	
		fgets( zones[i].descDiscourse, len, inputFile);	
		fscanf( inputFile, "%d", &zones[i].lenGrafFile );	
		if ( zones[i].lenGrafFile != -1 ){	
			zones[i].grafFile = (char*) malloc( sizeof ( char ) * ( zones[i].lenGrafFile + 1 ) );	
			t = getc( inputFile );	
			fgets( zones[i].grafFile, zones[i].lenGrafFile, inputFile );	
		}	
		/*printf("len = %d | disc: %s\n ", len, zones[i].descDiscourse );*/	
		fscanf(inputFile, "%d",&zones[i].movingOptions);	
		zones[i].toPoint = (int*) malloc( sizeof( int ) * zones[i].movingOptions );	
		/*printf("%d ",zonas[i].movingOptions);*/	
		for ( j = 0 ; j < zones[i].movingOptions ; ++j){	
			fscanf(inputFile, "%d",zones[i].toPoint+j);	
			/*printf("%d ",zonas[i].toPoint[j]);*/	
		}	
		/*printf("|\n\n");*/	
		fscanf(inputFile, "%d", &zones[i].boss);	
	}	
}
/*
Nombre: readGrafix
Entrada: una cadena 'nameFile' que almacena el nombre del archivo que contine el grafico imprimir
Descripcion: imprime el grafico del archivo.
*/
void readGrafix(char* nameFile){
	FILE* inputFile = fopen(nameFile, "r");
	if ( inputFile == NULL ){
		fprintf( stderr , "Error: el archivo no-mutable |%s| no pudo ser leido.\n", nameFile );
	}
	char c;
	while (fscanf(inputFile, "%c",&c) != EOF )
		printf("%c",c);
	fclose(inputFile);
}
