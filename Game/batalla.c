#include "batalla.h"

/*
Nombre: playerStatisticsToBattleAtack
Descripcion: transforma las estadisticas del jugador en las estadisticas que tienen los ataques del jugador.
*/
BattleAtack* playerStatisticsToBattleAtack(){
	BattleAtack* b = (BattleAtack*) malloc ( sizeof( BattleAtack ) * player.lenAtacks );
	int i, j;
	int acumStat;
	int variable;
	for ( i = 0 ; i < player.lenAtacks ; ++i ){
		b[i].stamina =  (player.atacks[i]).stamina;
		b[i].reactive = (player.atacks[i]).reactive;
		b[i].name = (player.atacks[i]).name;

		acumStat = 0;
		variable = 1;
		for ( j = 0 ; j < (player.atacks[i]).damageTerms ; ++j ){
			acumStat += variable * (player.atacks[i]).damageCalcFunction[j];
			variable *= (player.stats[(player.atacks[i]).damageStatDependence]).points;
		}
		b[i].damage = acumStat;

		acumStat = 0;
		variable = 1;
		for ( j = 0 ; j < (player.atacks[i]).probTerms ; ++j ){
			acumStat += variable * (player.atacks[i]).probCalcFunction[j];
			variable *= (player.stats[(player.atacks[i]).probStatDependence]).points;
		}
		b[i].prob = acumStat;
	}
	return b;
}

/*	
Nombre: batalla	
Entrada: un entero que representa el indice del jefe en la estructura 'bosses'	
Salida: un entero que toma 1 si se vencio al jefe correctamente, 0 de lo contrario.	
Descripcion: maneja la logica de la batalla, poniendo valores a estadisticas de batalla como el hp y 	
				stamina desde las 'estadisticas'. Determina los turnos y demas.	
*/
int batalla(int boss){
	int res;
	/* se inicializan todas las caracteristicas propias */
	BattleAtack* ataquesPropios = playerStatisticsToBattleAtack();
	readDiscourse(bosses[boss].entrance);

	int hp = calcHp((player.stats[0]).points);
	int stamina = calcStamina((player.stats[4]).points);
	int recuperacionStamina = calcRecuperacionStamina((player.stats[4]).points);

	int hpBoss = bosses[boss].hp;
	/* asuntos directamente de la batalla */
	int danoBoss, danoBossAxu, danoPropio, reactivo;

	/*Usado de invetario (buffs)*/
	BuffBatalla buff;
	buff.durationBuff = 0;
	buff.fuerzaBuff = 0;
	buff.reducionBoss = 0;
	buff.inmunidad = 0;
	float axu;

	while ((hp > 0) && (hpBoss > 0)){
		
		reactivo = turnoPropio(ataquesPropios, &danoPropio, &stamina, &hp, &buff, bosses[boss].name, hpBoss);
		/* para el efecto del pergamino maldito */
		axu = buff.reducionBoss;
		danoBossAxu = turnoBoss(boss, buff.inmunidad);
		danoBoss = danoBossAxu - (danoBossAxu * axu/100);

		descontarHp(&hp, &hpBoss, danoPropio, danoBoss, bosses[boss].name, reactivo);

		stamina += recuperacionStamina;
		/* duracion de los Buffs*/
		if(buff.durationBuff)
			--buff.durationBuff;
		if(!buff.durationBuff){
			buff.fuerzaBuff = 0;
			buff.reducionBoss = 0;
			buff.inmunidad = 0;
		}
		continuar();
	}
	/* si hpBoss < 0 && hp < 0, igual has muerto, por lo que da lo mismo: te ha vencido */
	if (hp > 0){
		/*printf("\nFelicitaciones por vencer a %s!!\n\n",bosses[boss].name);*/
		bosses[boss].defeated = 1;
		
		readDiscourse( bosses[boss].ending );

		player.improvableStats += bosses[boss].pointsKillingReward;
		printf("Has obtenido %d puntos de habilidad.\n", bosses[boss].pointsKillingReward );
		if ( player.nInventory < maxInv && bosses[boss].itemKillingReward != -1 ){
        	pushSort(&player.inventory, bosses[boss].itemKillingReward );
        	player.nInventory += 1;

			printf("Tambien has conseguido %s [%d/%d]\n", items[ bosses[boss].itemKillingReward ].name, player.nInventory, maxInv );
		}
		continuar();

	} else {
		printf("\n%s te ha vencido\n\n",bosses[boss].name);
		printf("Tu fin se acerca.\n");	
		res = 0;	
		deleteGame();
		--nProfiles;
		continuar();
	}	
	free( ataquesPropios );	
	return res;
}
/*
Nombre: tableroBatalla
Descripcion: muestra el tablero de batalla.
*/
void tableroBatalla(BattleAtack* ataquesPropios, int * stamina, int* hp, char * nombreBoss, int hpBoss){
	int i;
	system("cls");
	printf("\n-------------------------------------------------------------------------------------------\n");
	printf("Tienes %d de vida              -             %s tiene %d de vida\n",*hp,nombreBoss, hpBoss);
	printf("Tienes %d de stamina\n\n",*stamina);
	/* para mostrarle las opciones al usuario y que elija que ataque usar */
	printf("Escoge un ataque:\n");
	printf("codigo  dano  probabilidadDeImpacto  gastoDeStamina\n");
	for (i = 0 ; i < player.lenAtacks ; i++){
		if (*stamina >= ataquesPropios[i].stamina){
			printf("  [%d]   %4d            %3d%%                 %3d       %s\n",i,ataquesPropios[i].damage,ataquesPropios[i].prob,ataquesPropios[i].stamina, ataquesPropios[i].name);
		}
	}
	printf("===========================================================================================\n");
	printf("  [%d] Abrir invetario \n", i);
}

/*
Nombre: turnoPropio
Descripcion: le pregunta al usuario el movimiento que desea usar en su turno.
*/
int turnoPropio(BattleAtack* ataquesPropios,int* danoPropio, int * stamina, int* hp, BuffBatalla* buff, char * nombreBoss ,int hqBoss){
	int opcion, reactivo;
	float axu;
	tableroBatalla(ataquesPropios, stamina, hp, nombreBoss, hqBoss);
	do{	
		opcion = -1;
		opcion = inquirirOpcion(player.lenAtacks + 1);
		if(opcion == player.lenAtacks){
			inventarioBatalla(ataquesPropios, stamina, hp, buff);
			tableroBatalla(ataquesPropios, stamina, hp, nombreBoss, hqBoss);
		}
			
	} while (ataquesPropios[opcion].stamina > *stamina);

	/*printf("ESCOGISTES POR ALGUNA RAZON EL %d - %s",opcion,ataquesPropios[opcion].name);*/
	
	*stamina -= ataquesPropios[opcion].stamina;
	/* revisa si el ataque funciona con 'delay' como el parry o una futura esquivada */
	if (ataquesPropios[opcion].reactive == 1){
		axu = buff->fuerzaBuff;
		*danoPropio = (int)(ataquesPropios[opcion].damage + (ataquesPropios[opcion].damage * axu/100));
		reactivo = ataquesPropios[opcion].prob;
	} else{
		reactivo = 0;
		if (ataquesPropios[opcion].prob > probabilidad()){
			axu = buff->fuerzaBuff;
			*danoPropio =  (int)(ataquesPropios[opcion].damage + (ataquesPropios[opcion].damage * axu/100));
			printf("\nHas infligido %d de dano\n",*danoPropio);
		} else{
			printf("\nHas fallado tu ataque de %s\n",ataquesPropios[opcion].name);
			*danoPropio = 0;
		}

	}
	return reactivo;
}

void inventarioBatalla(BattleAtack* ataquesPropios, int* stamina, int* hp, BuffBatalla* buff){
	int i, opcion, item, j;
	int hpaxu = 0, staminaxu = 0;
	float axu;
	system("cls");
	fflush(stdin);
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
	printf("|[0] salir de inventario |[x] escoje el indice del objeto que deseas usar \n");
	opcion = inquirirOpcion(player.inventory.size + 1 );
	if(buff->durationBuff && opcion > 0){
		printf("Aun esta activo el efecto de %s :: [%d] turnos mas para poder usar otro item\n", items[buff->lastItem].name ,buff->durationBuff);
	}else if(opcion > 0){
		item = pop(&player.inventory, opcion - 1);
		printf("haz elegido %s\n", items[item].name);
		buff->lastItem = item;
		buff->durationBuff = items[item].duration;
		if(items[item].hpRec){
			axu = items[item].hpRec;
			hpaxu = (int)(*hp * axu/100);
			*hp += hpaxu;
		}
		if(items[item].staminaRec){
			axu = items[item].staminaRec;
			staminaxu = (int)(*stamina * axu/100);
			*stamina += staminaxu;
		}
		buff->fuerzaBuff = items[item].danoPot;
		buff->reducionBoss = items[item].redDamegeBoss;
		buff->inmunidad = items[item].inmunidad;
		--player.nInventory;
		printf("\n|========================|\n");
		printf("| vida       += %7d  |\n| stamina    += %7d  |\n| fuerza     += %7d %%|\n| AtaqueBoss -= %7d %%|\n", hpaxu, staminaxu, items[item].danoPot, items[item].redDamegeBoss);
		if(buff->inmunidad){
			printf("| Inmunidad  :     Activa|");
		}else{
			printf("| Inmunidad  :   InActiva|");
		}
		printf("\n");
		printf("|========================|\n");

	}
	continuar();
}

/*
Nombre: turnoBoss
Descripcion: hace que el boss escoja utilizando probabilidades un ataque a usar
*/
int turnoBoss(int boss, int inmunidad){
	int prob = probabilidad(),  i = 0, probAcum = 0, ans;
	do {	
		probAcum += (bosses[boss].atacks[i]).choosingProbability;	
		++i;	
	} while ( (probAcum < prob) && (i < bosses[boss].nAtacks) );	
	--i;

	if (!inmunidad && (bosses[boss].atacks[i]).impactProbability > probabilidad()){
		printf("\n%s te ataca con %s\n",bosses[boss].name, (bosses[boss].atacks[i]).name);
		ans = (bosses[boss].atacks[i]).damage;
	} else {
		printf("\n%s falla por poco su ataque de %s\n",bosses[boss].name, (bosses[boss].atacks[i]).name);
		ans = 0;
	}
	return ans;
}
/*
Nombre: descontarHp
Descripcion: descuenta la vida tanto al jugador como al boss. Toma en cuenta asuntos como 'parry'
*/
void descontarHp(int * hp, int * hpBoss, int danoPropio, int danoBoss, char* nombreBoss, int reactivo){
	if (reactivo == 0){
		*hp -= danoBoss;
		*hpBoss -= danoPropio;
		printf("\n%s te ha quitado %d de vida\n",nombreBoss,danoBoss);
	} else {
		if (danoBoss > 0){
			/* el que sea reactivo depende de la probabilidad de bloquarlo unicamente si fuiste atacado */
			if (probabilidad() < reactivo){
				*hpBoss -= danoPropio;
				printf("\nHas esquivado a %s, quitandole %d de vida en el proceso\n",nombreBoss,danoPropio);
			} else {
				*hp -= danoBoss;
				printf("\nHas recibido plenamente el ataque de %s. Te ha quitado %d de vida\n",nombreBoss,danoBoss);
			}
		} else {
			printf("\n%s ha fallado. No hizo falta una defensa\n",nombreBoss);
		}
	}
}
/*
Nombre: calcRecuperacionStamina
Descripcion: calcula la recuperacion de stamina por turno segun las estadisticas
*/
int calcRecuperacionStamina(int puntos){
	int ans = puntos * 2;
	ans += 10;
	return ans;
}
/*
Nombre: calcStamina
Descripcion: calcula la stamina total segun las estadisticas
*/
int calcStamina(int puntos){
	int ans = puntos * 15;
	ans += 250;
	return ans;
}
/*
Nombre: calcHp
Descripcion: calcula la salud total segun las estadisticas
*/
int calcHp(int puntos){
	int ans = puntos * 250;
	ans += 500;
	return ans;
}

