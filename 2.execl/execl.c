#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {

	pid_t ret;
	int stato;
	ret = fork();

	printf("pid=%d \n", getpid());

	if (ret == 0) {
    // processo figlio
    // cambia l'immagine core con quello di "nuovo"
		
    // esegui il programma "nuovo" e passagli queste stringhe
    execl("./nuovo", "Saluti", " dal processo", " padre", (char*)0);
		
    // se tutto va bene con il cambio di immagine queste istruzioni non
    // saranno eseguite, al posto loro verranno eseguite quelle di "nuovo"
    printf("exec fallita\n");
		exit(1);
	}
	else if (ret > 0) {
    // processo padre
    //
		printf("sono il padre con pid=%d\n", getpid());
		
    // aspetta il  figlio
    // memorizza lo stato in "stato"
    ret = wait(&stato);

    printf(" %d \n", stato>>8);
	}
	else 
		printf("Errore fork");
				
}
