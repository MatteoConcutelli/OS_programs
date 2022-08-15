#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define DIM 256
#define LEGGI 0
#define SCRIVI 1

int main() {
  
  int n, fd[2];
  int pid; 
  char messaggio[DIM];

  if (pipe(fd) < 0) {
    printf("errore pipe");
    exit(1);
  }

  if ( (pid = fork()) < 0) {
    printf("errore fork");
    exit(1);
  }
  else if (pid > 0) {
    /* padre */
    close(fd[LEGGI]); //chiude il canale che non usa
    write(fd[SCRIVI], "Ciao, figlio", DIM);
  }
  else {
    /* figlio */
    close(fd[SCRIVI]);  // chiude il canale che non usa
    n = read(fd[LEGGI], messaggio, DIM);
    printf("%s\n", messaggio);
  }
}

