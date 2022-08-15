#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char *argv[]) {

	printf("Processo chiamato da execl con PID = %d e PPID = %d\n", getpid(), getppid());
	
	for ( int i = 0; i < argc; i++) 
		printf("Parametri ricevuti dal figlio : \n	%s ", argv[i]);	// visualizza i parametri d'ingresso
	
	printf("\n");
}
