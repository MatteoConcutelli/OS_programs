#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FINE (-1)

#define MAX 20
#define DIM 10


/*  Esempio problema prod cons. Più consumatori possono utilizzare un buffer
 *  in grado di contenere, al massimo, N messaggi.
 *  Ricordiamo che i vincoli per accedere al buffer sono 2:
 *    - il produttore non può inserire un messaggio nel buffer se questo
 *      è pieno.
 *    -Il consumatore non può prelevare dal buffer se questo è vuoto
 * 
 *  I messaggi sono dei valori interi, realizziamo il buffer come un vettore di
 *  interi, gestendolo in modo circolare. per gestire il buffer associamo ad
 *  esso ule seguenti variabili:
 *    - cont, numero dei messaggi contenuti nel buffer;
 *    - scrivi, indice del prossimo elemento da scrivere;
 *    - leggi, indice del prossimo elemetno da leggere
 *
 *  Essenfo il buffer una risorsa condivisa è necessario associare ad esso un
 *  mutex M per controllarne l'accesso in mutua esclusione.
 *  Il consumatore ed il produttore devono anche sospendersi nel caso il buffer
 *  sia pieno o vuoto rispettivamente.
 */

typedef struct {
  
  pthread_mutex_t M;
  pthread_cond_t PIENO;
  pthread_cond_t VUOTO;
  int messaggio[DIM];
  int leggi, scrivi;
  int cont;
}buffer_t;

buffer_t buf;

void init (buffer_t *buf);
void produci (buffer_t *buf, int mes);
int consuma (buffer_t *buf);

void init(buffer_t *buf) {

  pthread_mutex_init (&buf->M, NULL); 
  pthread_cond_init (&buf->PIENO, NULL);
  pthread_cond_init(&buf->VUOTO, NULL);

  buf->cont = 0;
  buf->leggi = 0;
  buf->scrivi = 0;
  
}

void produci (buffer_t *buf, int mes) {
  pthread_mutex_lock (&buf->M);
  if (buf->cont == DIM) // il buffer è pieno ? 
    pthread_cond_wait(&buf->PIENO, &buf->M);

  buf->messaggio[buf->scrivi] = mes;
  buf->cont++;
  buf->scrivi++;

  //la gestione del buffer è circolare
  if (buf->scrivi == DIM) buf->scrivi = 0;

  pthread_cond_signal (&buf->VUOTO);
  pthread_mutex_unlock(&buf->M);
}

int consuma (buffer_t *buf) {
  int mes;
  pthread_mutex_lock(&buf->M);
  if (buf->cont == 0) // il buffer è vuoto ?
    pthread_cond_wait(&buf->VUOTO, &buf->M);

  mes = buf->messaggio[buf->leggi];
  buf->cont--;
  buf->leggi++;

  // la gestione è circolare
  if (buf->leggi >= DIM) buf->leggi = 0;
  // risveglia un eventuale thread produttore
  pthread_cond_signal(&buf->PIENO);
  pthread_mutex_unlock(&buf->M);
  return mes;
}

void *produttore (void *arg) {
  int n;
  for (n = 0; n < MAX; n++) {
    printf("produttore %d -> %d\n", (int)arg, n);
    produci(&buf, n);
    sleep(1);
  }
  produci(&buf, FINE);
}

void *consumatore (void *arg) {
  int d;
  while (1) {
    d = consuma(&buf);
    if (d == FINE) break;
    printf("  %d <- consumatore %d\n", d, (int)arg);
    sleep(2);
  }
}

int main() {
  int i;
  int nprod = 1, ncons = 1;
  pthread_t prod[nprod], cons[ncons];
  init(&buf);

  // creazione thread
  for (i = 0; i < nprod; i++)
    pthread_create(&prod[i], NULL, produttore, (void*)i);

  for (i = 0; i < ncons; i++)
    pthread_create(&cons[i], NULL, consumatore, (void*)i);

  // Attesa dei thread creati 
  for (i = 0; i < nprod; i++) 
    pthread_join(prod[i], NULL);
  for (i = 0; i <ncons; i++)
    pthread_join(cons[i], NULL);

  return 0;
}
