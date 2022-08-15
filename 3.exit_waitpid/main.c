#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main()  {
	
	pid_t ret, pid;
	int stato;
	
	ret = fork();

	if (ret == 0) {
		// codice del figlio
		printf("Sono il figlio pid: %d \n", getpid());
		sleep(10);	//sospensione per 10 secondi
		exit(2);	// valore che il padre leggerà in stato
	}
	else if (ret > 0) {
		// codice del padre
		pid = wait(&stato);
		printf("processo figlio pid: %d terminato \n", pid);

		if (stato < 256)
			printf("Terminazione forzata : segnale = %d \n", stato);
		else 
			printf("terminazione volontaria : stato = %d \n", stato>>8);
	}
	else 
		printf("fork fallita");

}

/*
	se la terminazione del figlio è :
		-volontaria lo stato assume	un valore sul secondo 
		 byte meno significativo, per leggerlo quindi facciamo 
		 uno shift a destra di un byte.
		
		-forzata lo stato assume un valore sul byte meno significativo
		 quindi basta leggerlo.
*/
