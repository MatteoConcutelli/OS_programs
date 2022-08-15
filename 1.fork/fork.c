#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
	
	pid_t ret = fork();
	/*
	 * fork ritorna il pid del figlio al padre,
	 * il valore 0 al figlio. 
   * -1 in caso di errore.
	 *
	 * questi IF separano il codice eseguito dal padre 
	 * con quello eseguito dal figlio
	 * */

	if (ret == -1) {
		printf("Errore fork");
		exit(0);
	} else if (ret == 0) {
		// codice del figlio
		printf("figlio con pid= %d \n", getpid());	
	} else {
		// codice del padre
		printf("padre con pid = %d\n", getpid());
	}

	// codice eseguito da entrambi i processi, padre e figlio
	printf("Questa istruzione è eseguita da pid = %d \n", getpid());

}
