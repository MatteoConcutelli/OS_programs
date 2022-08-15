#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int A = 10; // var condivisa
pthread_mutex_t M;

void *codiceTh1 (void *arg) {
  
  for (int i = 0; i < 10; i++) {
    pthread_mutex_lock(&M);
    printf("Thread : %s", (char*)arg);
  
    A--;

    printf("A = %d\n", A);
    pthread_mutex_unlock(&M);
    sleep(1);
  }
}
void *codiceTh2 (void *arg) {

  for (int i = 0; i < 10; i++) {
    pthread_mutex_lock(&M);
    printf("Thread: %s", (char*)arg);
    
    A++;

    printf("A = %d\n", A);
    pthread_mutex_unlock(&M);
    sleep(1);
  }
}


void main () {
  pthread_t th1, th2;

  pthread_create(&th1, NULL, codiceTh1, "th1");
  pthread_create(&th2, NULL, codiceTh2, "th2");

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
}
