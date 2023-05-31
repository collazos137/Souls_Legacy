#include "menu.h"

/*
Nombre: getName

*/
int getName(char** name){	
	List auxL = createList();	
	char t;	
	printf("Escribe el nombre de tu personaje:\n> ");	
	int i = 1;	
	fflush( stdin );	
	t = getc( stdin );	
	while ( t != '\n'){	
		++i;	
		append( &auxL, t );	
		t = getc( stdin );	
	}	
	(*name) = (char*) malloc( sizeof ( char ) * i );	
	Node *tmp = auxL.firts;	
	i = 0;	
	while ( tmp != NULL ){	
		(*name)[i] = tmp->date;	
		tmp = tmp->next;	
		++i;	
	}	
	(*name)[i] = '\0';	
	kill( &auxL );	
	return i+1;	
}
/*
#########################################################################################################################
#########################################################################################################################
###################################### El menu inicial (cuando se carga el perfil) ######################################
#########################################################################################################################
#########################################################################################################################
*/

/*
Nombre: chosseClass
Entrada: Ninguna
Descripcion: Menu para elegir la clase de jugador
*/
void choseClass(){
	int i, j;
	system("cls");
	readGrafix("archivos\\graficos\\graMenuClases.txt");
	for(i = 0; i < nClases; ++i){
		printf("|[%d] %s", i, (clases[i]).name);
	}
	printf("|%*s|\n", (player.stats[1]).lenName + 3, "Estadisticas");
	for(i = 0; i < player.lenStats; ++i){
		for(j = 0; j < nClases ; ++j){
			printf("|%*i", (clases[j]).lenName + 3, (clases[j]).stats[i]);
		}
		printf("|%*s|\n",(player.stats[1]).lenName + 3, (player.stats[i]).name);
	}
	printf("==================================================================================\n");
	printf("=== (Al iniciar con una clase obtendras la bonificacion escifica de esa clase) ===\n");
	printf("==================================================================================\n");
	int option = inquirirOpcion(nClases);
	system("cls");
	printf("has elegido la clase %s\n", clases[option].name);
	printf("|======================|\n");
	for(i = 0; i < player.lenStats; ++i){
		(player.stats[i]).points = (clases[option]).stats[i];
		printf("|%*s : +%-2d|\n", (player.stats[1]).lenName, (player.stats[i]).name, (clases[option]).stats[i]);
	}
	printf("|======================|\n");
	continuar();
}


void createNewGame ( char* ending ){	
	Profiles* tmp = (Profiles*) malloc( sizeof ( Profiles ) );	
	tmp->nameFile = (char*) malloc( sizeof( char ) * lenPredFile );	
	modifyEnding( ending );	
	int aux = 0;	
	strcpy( tmp->nameFile, profileFilePrefix );	
	aux += strlen( profileFilePrefix );	
	strcpy( tmp->nameFile + aux, ending );	
		
	aux += strlen( ending );	
		
	strcpy( tmp->nameFile + aux, profileFileExtension );	
	/*printf("nameFile = %s\n", tmp->nameFile );*/	
	tmp->lenNamePlayer = getName(&tmp->namePlayer);	
	tmp->lenFile = lenPredFile;	
	tmp->next = perf;	
	perf = tmp;	
	++nProfiles;	
	writeProfiles( ending );	
	profileFile = tmp->nameFile;	
	/*printf("%s\n",profileFile);*/	
	resetInfo();	
	choseClass();	
	overwriteGame( profileFile );	
	player.lenName = tmp->lenNamePlayer;	
	player.name = tmp->namePlayer;	
	readDiscourse( zones[myZone].descDiscourse );	
	menuPrincipal();	
}
/*
Nombre: bigMenu
Entrada: dos cadenas de caracteres 'ending' (para evitar colisiones en el nombre de los archivos de usuario) y
			'allProfiles' que representa el nombre del archivo que contiene los perfiles
*/
void bigMenu( char* ending  ){
	int option, flag = 1;
	int i;
	while ( flag ){
		system("cls");
		readGrafix("archivos\\graficos\\graMenu.txt");
		printf("Selecciona una opcion:\n");
		
		i = 0;
		printf("[%d] juegoNuevo\n", i);
		++i;
		if ( nProfiles != 0 ){
			printf("[%d] continuar\n",i );
			++i;
		}
		printf("[%d] salir\n",i);
		++i;

		option = inquirirOpcion( i );
		continuar();
		if ( option == 0 ){
			createNewGame( ending );
		} else if ( option == 1 && nProfiles != 0 ){
			profileSelectionMenu( );
		} else {
			flag = 0;
		}
	}
	
}
/*
Nombre: profileSelectionMenu
Descripcion: es el menu mediante el cual el jugador escoge el perfil que quiere cargar
*/
void profileSelectionMenu (  ){
	Profiles* tmp;
	printf("Escoge tu perfil:\n[0] regresar al menu\n");
	tmp = perf;
	int i = 1 ;
	while ( tmp != NULL ){
		printf("[%d] %s\n",i, tmp->namePlayer );
		tmp = tmp->next;
		++i;
	}
	printf("\n");
	int opcion = inquirirOpcion(nProfiles + 1);
	if ( opcion != 0 ){
		tmp = perf;
		i = 1;
		while ( i != opcion ){
			tmp = tmp->next;
			++i;
		}
		profileFile = tmp->nameFile;
		readGame( profileFile );
		player.lenName = tmp->lenNamePlayer;
		player.name = tmp->namePlayer;

		menuPrincipal();
	} else {
		continuar();
	}
}


/*
#########################################################################################################################
#########################################################################################################################
###################################### El menu principal ################################################################
#########################################################################################################################
#########################################################################################################################
*/

	void menuPrincipal(){	
	/*================================================================*/	
	/*variables de logica*/	
	int opcion;	
	int flag = 1;	
	/*================================================================*/	
	while (flag){	
		system("cls"); /*Borra lo anterior en consola*/	
		fflush(stdin); /*limpia el la entrada de datos por consola*/	
		if ( zones[ myZone ].lenGrafFile != -1 ){	
			readGrafix( zones[ myZone ].grafFile );	
		}	
		printf("==================================================================================\n");	
		printf("================================= Menu principal =================================\n");	
		printf("|[1] Explorar|[2] Avanzar|[3] Estadisticas|[4] Inventario|[5] Bestiario|[6] Salir|\n");	
		printf("==================================================================================\n");	
		printf("ingrese una opcion: ");	
		scanf("%d", &opcion);/*captura una opcion a elegir*/	
		system("cls");	
		switch(opcion){	
			case 1:	
				zones[myZone].explored = 1;	
				if ( zones[myZone].boss == -1 || bosses[zones[myZone].boss].defeated ){	
					explorar();	
					printf("======================================================================\n");	
					continuar();	
				} else {	
					flag = batalla( zones[myZone].boss );	
				}	
				break;	
			case 2:	
				moverseEntreZonas();	
				break;	
			case 3:	
                menuEstadisticas();	
                printf("======================================================================\n");	
				continuar();	
				break;	
			case 4:	
				mostrarInventario();	
				printf("======================================================================\n");	
				continuar();	
				break;	
			case 5:	
				bestiario();	
				printf("======================================================================\n");	
				continuar();	
				break;	
			case 6:	
				flag = 0;	
				break;	
			default:	
				printf("opcion no valida\n");	
		}	
			
	}	
}

/*===============================================================================================================================================================================================================================*/
/* 1.explorar (Opcion de menu principal)*/

/*
Nombre: explorar
Descripcion: dependiendo del valor que retorne la funcion probalibidad() se redirige a otras funciones especificas, que representan los posibles resultados que tiene "explorar". 
*/
void explorar(){
	printf("============================ Exploracion =============================\n");
	printf("======================================================================\n");
	int proba = probabilidad();
	if (proba <= 20){
		antorcha();
	}else if(proba <= 35){
		encontarPuntoHabilidad();
	}else if(proba <= 50){
		objetos();
	}else{
		printf("has tenido mala suerte :(\n");
	}
}

/*
Nombre: antorcha
Descripcion: muestra texto al jugador en pantalla y le pregunta si desea guardar el juego. 
*/
void antorcha(){
	char opcion;
	int flag = 1;
	int op;
	printf("El aventurero a encontrado una antorcha!!!!!\n");
	printf("Desea guardar la partida (y/n): ");
	while (flag){
		fflush(stdin);
		scanf("%c", &opcion);

		if(opcion == 'y'){
			printf("Guardando la partida.....\n");
			overwriteGame( profileFile );
			flag = 0;
		}else if(opcion == 'n'){
			printf("OK.\n");
			flag = 0;
		}else{
			printf("___________________________________\n");
			printf("opcion no valida, intente de nuevo:\n");
			printf("Desea guardar la partida (y/n): ");
		}
	}

}
/*
Nombre: objetos
Descripcion: dependiendo del valor que retorne probalidad añade un tipo item al inevntario y muestra en pantalla en nombre del item y su rareza . 
*/
void objetos(){
	if(player.nInventory < maxInv ){
		int i = 0, probabilidadAcum = items[0].probEncontrar;
		int proba = probabilidad();
		printf("has encontrado un objeto: ");

		while ( probabilidadAcum < proba ){
			probabilidadAcum += items[++i].probEncontrar;
		}
		
        pushSort(&player.inventory, i);
        /*printf("nInventory = %d\n",player.nInventory);*/
        player.nInventory += 1;

		printf("%s", items[ i ].name );
		printf(" | rareza[%d] (para mas informacion mira tu inventario).", items[i].rareza );
		printf("(%d/%d)\n",player.nInventory, maxInv);

	}else{
		printf("has encontrado un objeto, pero no queda espacio en el inventario\n");
	}
}
/*
Nombre: encontrarPuntoHabilidad
Descripcion: le dice al jugador que ha encontrado un punto de habilidad y lo actualiza en los datos del jugador.
*/
void encontarPuntoHabilidad(){
    player.improvableStats += 1;
	printf("El Explorador encontro un punto de habilidad (+ 1) [%d]\n", player.improvableStats);
}

/*===============================================================================================================================================================================================================================*/
/* 2. explorar (opcion de menu principal)
*/

/*
Nombre: moverseEntreZones
Descripcion: le pide al usuario que elija una de las posibles zonas a las que puede moverse.
				Si la zona que el usuario elige tiene un boss vinculado, inicia la pelea con el boss.
*/
void moverseEntreZonas(){
	int opcion, opcionExplorar;
	int i;
	printf("Estas en: %s\n",zones[myZone].name);
	printf("Elige una de las siguientes opciones para moverte:\n[0] quedarme\n");
	for ( i = 0 ; i < zones[myZone].movingOptions ; ++i){
		if ( zones[myZone].explored || zones[zones[myZone].toPoint[i]].explored ){
			printf("[%d] %s\n",i+1, zones[zones[myZone].toPoint[i]].name);
		}
	}
	printf("\n");
	do{
		opcionExplorar = inquirirOpcion(zones[myZone].movingOptions+1);
	} while ( opcionExplorar != 0 && !zones[opcionExplorar-1].explored && !zones[myZone].explored );
	continuar();
	if ( opcionExplorar != 0 ) {
		myZone = zones[myZone].toPoint[opcionExplorar-1];
		readDiscourse( zones[myZone].descDiscourse );
	}
}

/*===============================================================================================================================================================================================================================*/
/*3. Estadiaticas (Opcion del menu principal)*/
/*
Nombre: menuEstadisticas 
Descripcion: muestra las estadisticas del jugador en pantalla y le permite incermentarlas con puntos de habilidad. 
*/

void menuEstadisticas(){
	int i, opcion;
	printf("============================ Estadisticas ============================\n");
	printf("======================================================================\n");
	printf("Tienes %d puntos disponibles para mejorar.\n",player.improvableStats);
	printf("======================================================================\n");
	printf(" Nombre | ");
	for ( i = 0 ; i < player.lenStats ; i++ ){
		printf("%s | ", (player.stats[i]).name );
	}
	printf("\n");

	printf(" Puntos | ");
	for ( i = 0 ; i < player.lenStats ; i++ ){
		printf("%*d | ",( player.stats[i]).lenName -1, (player.stats[i]).points );
	}
	printf("\n");
	printf("======================================================================\n");
	printf("[0] Salir | [1] Mejorar habilidad\n");
	opcion = inquirirOpcion( 2 );

	if ( opcion == 1 ){
		if ( player.improvableStats == 0 ){
			printf("No tienes suficientes puntos para mejorar tus habilidades.\n");
		} else {
            fflush(stdin);
            system("cls");
			mejorarEstadisticas();
		}	
	}	
}
/*
Nombre: mejorarEstadisticas
Descripcion: le pide al usuario por la informacion necesaria para actualizar las estadisticas con los puntos de 
				habilidad acumulados.
*/
void mejorarEstadisticas ( ){
	int opcion, i, aumento;
	printf("Escoge la habilidad que quieres mejorar:\n");
	printf("codigo   puntosObtenidos   nombreHabilidad\n");
	for ( i = 0 ; i < player.lenStats ; i++ ){
		printf("  [%d]            %d            %s\n",i, (player.stats[i]).points ,(player.stats[i]).name);
	}
	opcion = inquirirOpcion( player.lenStats );
	fflush(stdin);
    system("cls");
	printf("\nEscribe los puntos que deses agregarle a tu %s - tienes %d disponibles\n", (player.stats[opcion]).name, player.improvableStats );
	aumento = inquirirOpcion ( player.improvableStats +1 );
	(player.stats[opcion]).points += aumento;
	player.improvableStats -= aumento;
}

/*===============================================================================================================================================================================================================================*/
/*4. Inventario (Opcion del menu principal)*/
/*
Nombre: mostrarInventario
Descripcion: muestra el inventario del jugador en pantalla. 
*/
void mostrarInventario(){
	int i;
	printf("============================ Inventario ==============================\n");
	printf("======================================================================\n");
	printf("----------------------------------------------------------------------\n");
    Node* tmp = player.inventory.firts;
	for(i = 0; i < maxInv; ++i){
		printf("%2d. ",(i+1));
		if(tmp != NULL){
            printf("%s", items[ tmp->date ].name );
            tmp = tmp->next;
		}else{
			printf("vacio");
		}
		printf("\n");
		printf("----------------------------------------------------------------------\n");
	}
}
/*===============================================================================================================================================================================================================================*/
/*5. bestiario (Opcion del menu principal)*/

/*
Nombre: bestiario
Descripcion: muestra la informacion de los jefes derrotados.
*/
void bestiario(){
	int i, flag = 1, opcion;
	printf("============================= Bestiario ==============================\n");
	printf("======================================================================\n");
	printf("Jefes derrotados:\n");
	printf("----------------------------------------------------------------------\n");
	for(i = 0; i < 5; ++i){
		if( bosses[i].defeated ){
			printf("%d. |||%s|||\n", i+1, bosses[i].name);
			printf("vida:%d\n", bosses[i].hp);
			imprimirDescripcion(bosses[i].description);
			printf("----------------------------------------------------------------------\n");
		}
	}
}

/*
Nombre: imprimirDescripcion
Descripcion: imprime la descripcion de los jefes derrotados de manera justificada.
*/
void imprimirDescripcion(char* descripcion){
	int i = 0, cont = 0;
	printf("Descripcion:\n");
	while(descripcion[i] != '\0'){
		printf("%c", descripcion[i]);
		if (cont == 69){
			printf("\n");
			cont = 0;
		}
		++cont;
		++i;
	}
	printf("\n");
}
