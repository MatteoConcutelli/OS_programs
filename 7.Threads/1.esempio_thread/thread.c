#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 10;
char buffer[1024];

void* codice_Th1 (void *arg) {
  
  // codice 

  pthread_exit(0);
}

void* codice_Th2 (void *arg) {
  
  // codice
  
  pthread_exit(0);
}

int main() {

  pthread_t th1, th2;
  int ret;
  
  // creazione e attivazione del primo thread
  if (pthread_create(&th1, NULL, codice_Th1, "Lino") != 0) {
    fprintf(stderr, "Erorre di creazione thread 1 \n");
    exit(1);
  }
  
  // creazione e attivazione del secondo thread
  if (pthread_create(&th2, NULL, codice_Th2, "Eva") != 0) {
    fprintf(stderr, "Errore di creazione thread 2 \n");
    exit(1);
  }

  ret = pthread_join(th1, NULL); // aspetta terminazione th1

  if (ret != 0) 
    fprintf(stderr, "join fallito %d \n", ret);
  else 
    printf("terminato il thread 1 \n");

  ret = pthread_join(th2, NULL);
  if (ret != 0)
    fprintf(stderr, "join fallito %d\n", ret);
  else 
    printf("terminato il thread 2\n");

  return 0;

}
