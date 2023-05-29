#include "manejoArchivos.h"
#include "usosVariados.h"
#include "batalla.h"
#include "menu.h"
#include "lista.h"
#include "estructurasGenerales.h"


int main(){
	allProfiles = "archivos\\perfiles.txt";
	nameNonMutable = "archivos\\informacionNoMutable.txt";

	profileFilePrefix = "archivos\\perfiles\\soulsProfile";
	profileFileExtension = ".txt";

	readNonMutableFile();

	perf = NULL;
	char ending[5];

	readProfiles( ending );
	
	player.inventory = createList();
	/*printProfiles(perf);*/

	bigMenu( ending );
	return 0;
}


