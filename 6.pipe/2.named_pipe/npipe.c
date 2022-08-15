#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

/* Quì il produttore invia al consumatore (half-duplex, cioè unidirezionale) -> una FIFO. 
   Se volevo una comunicazione full-duplex (bidirezionale) 
   avevo bisogno di due FIFO
*/
struct message_t {
  int numero;
  char text[64];
}msg;

int main (int argc, char *argv[]) {
  
  int fd, i = 0, n = 10;
  int size = sizeof(msg);
  srand(time(NULL));

  if (argc == 2) {
    mknod("mia_pipe", S_IFIFO | 0666, 0); //crea file FIFO (named pipe), nella cartella corrente
    fd = open("mia_pipe", O_WRONLY);  // produttore bloccato fino a quando il consumatore non apre mia_pipe in lettura
    if (fd < 0) {
	printf("Prod: Errore apertura pipe\n");
	exit(-1);    
    }
  }
  else {
    fd = open("mia_pipe", O_RDONLY);  // consumatore
    if (fd < 0) {
	printf("Cons: Errore apertura pipe\n");
	exit(-1);    
    }
  }

  strcpy(msg.text, "Auguri");

  for (i = 0; i < n; i++) {
    if (argc == 2) {
      // produttore
      msg.numero = rand()%100+1;
      printf("produttore: %s %d \n", msg.text, msg.numero);
      write(fd, &msg, size);  // scrive sulla pipe
      sleep(1);
    }
    else {
      // consumatore
      read(fd, &msg, size); // legge dalla pipe
      printf("consumatore : %s %d\n", msg.text, msg.numero);
      sleep(1);
    }
  }
  
  //close(fd);	// chiamate per i file: open, read, write, close.

  unlink("mia_pipe");	// cancella la pipe (il file "mia_pipe" nella directory corrente);
  // se non si fa questo la pipe rimarrà in memoria nella directory
  // le pipe con nome continuano ad esistere anche dopo che i processi, che
  // la usano, terminano
  
}
