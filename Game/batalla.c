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
Descripcion: maneja la logica de la batalla, poniendo valores a estadisticas de batalla como el hp y 
				stamina desde las 'estadisticas'. Determina los turnos y demas.
*/
void batalla(int boss){
	/* se inicializan todas las caracteristicas propias */
	BattleAtack* ataquesPropios = playerStatisticsToBattleAtack();
	readDiscourse(bosses[boss].entrance);

	int hp = calcHp((player.stats[0]).points);
	int stamina = calcStamina((player.stats[4]).points);
	int recuperacionStamina = calcRecuperacionStamina((player.stats[4]).points);

	int hpBoss = bosses[boss].hp;
	/* asuntos directamente de la batalla */
	int danoBoss, danoPropio, reactivo;
	while ((hp > 0) && (hpBoss > 0)){
		printf("\n-------------------------------------------------------------------------------------------\n");
		printf("Tienes %d de vida              -             %s tiene %d de vida\n",hp, bosses[boss].name, hpBoss);
		printf("Tienes %d de stamina\n\n",stamina);
		reactivo = turnoPropio(ataquesPropios, &danoPropio, &stamina);

		/* meter lo de inventario - pociones*/

		danoBoss = turnoBoss(boss);
		descontarHp(&hp, &hpBoss, danoPropio, danoBoss, bosses[boss].name, reactivo);
		stamina += recuperacionStamina;
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
	}
}
/*
Nombre: turnoPropio
Descripcion: le pregunta al usuario el movimiento que desea usar en su turno.
*/
int turnoPropio(BattleAtack* ataquesPropios, int * danoPropio, int * stamina){
	int i, opcion, reactivo;
	
	/* para mostrarle las opciones al usuario y que elija que ataque usar */
	printf("Escoge un ataque:\n");
	printf("codigo  dano  probabilidadDeImpacto  gastoDeStamina\n");
	for (i = 0 ; i < player.lenAtacks ; i++){
		if (*stamina >= ataquesPropios[i].stamina){
			printf("  [%d]   %4d            %3d%%                 %3d       %s\n",i,ataquesPropios[i].damage,ataquesPropios[i].prob,ataquesPropios[i].stamina, ataquesPropios[i].name);
		}
	}
	/*para evitar casos de que escojan uno que no puedan realizar*/
	do{
		opcion = inquirirOpcion(player.lenAtacks);
	} while (ataquesPropios[opcion].stamina > *stamina);

	/*printf("ESCOGISTES POR ALGUNA RAZON EL %d - %s",opcion,ataquesPropios[opcion].name);*/
	
	*stamina -= ataquesPropios[opcion].stamina;
	/* revisa si el ataque funciona con 'delay' como el parry o una futura esquivada */
	if (ataquesPropios[opcion].reactive == 1){
		*danoPropio = ataquesPropios[opcion].damage;
		reactivo = ataquesPropios[opcion].prob;
	} else{
		reactivo = 0;
		if (ataquesPropios[opcion].prob > probabilidad()){
			*danoPropio = ataquesPropios[opcion].damage;
			printf("\nHas infligido %d de dano\n",*danoPropio);
		} else{
			printf("\nHas fallado tu ataque de %s\n",ataquesPropios[opcion].name);
			*danoPropio = 0;
		}

	}
	return reactivo;
}
/*
Nombre: turnoBoss
Descripcion: hace que el boss escoja utilizando probabilidades un ataque a usar
*/
int turnoBoss(int boss){
	int prob = probabilidad(),  i = -1, probAcum = 0, ans;
	while ((probAcum < prob) && (i < bosses[boss].nAtacks)){
		++i;
		probAcum += (bosses[boss].atacks[i]).choosingProbability;
	}

	if ((bosses[boss].atacks[i]).impactProbability > probabilidad()){
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

